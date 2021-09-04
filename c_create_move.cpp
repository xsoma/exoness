#include "csgo_hooks.h"
#include "cvar_use.h"
#include "menu.h"
#include "prediction.h"
#include "mathematics.h"
#include "movement.h"
#include "rage_bot.h"
#include "antiaim.h"
#include "animations.h"
#include "miss_count.h"

using create_move_t = bool(__thiscall*)(IClientMode*, float, CUserCmd*);

bool __stdcall hooks::create_move(float smt, CUserCmd* m_pcmd) {

	if (!m_pcmd || !m_pcmd->command_number)
		return true;

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return true;

	game::main.send_packet = true;
	game::main.fake_lagging = false;

	game::main.command = m_pcmd;
	game::main.m_local = reinterpret_cast<c_base_entity*>(interfaces::entitylist->GetClientEntity(interfaces::engine->GetLocalPlayer()));

	if (!game::main.get_local())
		return true;

	uintptr_t* frame_ptr;
	__asm mov frame_ptr, ebp;

	anti_aim::get().fake_lag_limit();
	anti_aim::get().fake_lag_start();

	Vector original = m_pcmd->viewangles;

	cvar::get().on_create_move();

	if (c_config::get().bhop)
		m_movement::get().run_bhop(m_pcmd);
	
	m_movement::get().run_strafe(m_pcmd, c_config::get().strafe_type);

	if (game::main.get_local()->is_alive())
		if (GetAsyncKeyState(c_config::get().slow_key))
			m_movement::get().run_slow_walk(m_pcmd, c_config::get().slow_walk);

	c_animations::get().player_animations();

	if (c_config::get().aimbot_enabled)
	{
		c_prediction::get().run_prediction(m_pcmd);
		{
			anti_aim::get().on_create_move(m_pcmd);
			miss_count::get().build_();
			c_rage::get().on_create_move(m_pcmd);
		}
		c_prediction::get().finish();
	}
	
	c_rage::get().exploits_run();

	if (c_config::get().chams_fake)
		c_animations::get().manage_fake_animations();

	utils::fix_movement(original, m_pcmd);
	math::normalize_angles(m_pcmd->viewangles);
	math::clamp_angles(m_pcmd->viewangles);

	if (interfaces::engine->GetNetChannel()->m_nChokedPackets > 14)
		game::main.send_packet = true;

	if (game::main.send_packet)
		game::main.real_angle = m_pcmd->viewangles;

	*(bool*)(*frame_ptr - 0x1C) = game::main.send_packet;
	
	return false;
}