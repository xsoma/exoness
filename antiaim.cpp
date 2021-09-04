#include "antiaim.h"
#include "menu.h"
#include "mathematics.h"
#include <algorithm>

bool need_to_flip = false;

float get_curtime(CUserCmd* ucmd) {
	auto local_player = game::main.get_local();

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->m_nTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces::global_vars->interval_per_tick;
	return curtime;
}

bool next_lby_update(const float yaw_to_break, CUserCmd* cmd)
{
	auto local_player = game::main.get_local();

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->get_animation_state();
	if (!animstate)
		return false;

	if (!(local_player->m_fFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

void anti_aim::on_create_move(CUserCmd* cmd)
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (!game::main.get_local()->is_alive())
		return;

	if (!game::main.get_local()->m_hActiveWeapon().Get()) 
		return;
	
	if (game::main.get_command()->buttons & IN_USE) 
		return;
	
	if (game::main.get_local()->m_bGunGameImmunity() || game::main.get_local()->m_fFlags() & FL_FROZEN)
		return;

	float flServerTime = game::main.get_local()->m_nTickBase() * interfaces::global_vars->interval_per_tick;
	bool canShoot = (game::main.get_local()->m_hActiveWeapon().Get()->m_flNextPrimaryAttack() <= flServerTime);

	if ((canShoot && (cmd->buttons & IN_ATTACK)) && game::main.get_local()->m_hActiveWeapon().Get()->m_iItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
		return;
	
	if (game::main.get_local()->m_hActiveWeapon().Get()->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		if (game::main.get_command()->buttons & IN_ATTACK2)
			return;

		if (game::main.get_local()->m_hActiveWeapon().Get()->can_fire_post_pone() && (game::main.get_command()->buttons & IN_ATTACK))
			return;
	}
	else if (game::main.get_local()->m_hActiveWeapon().Get()->is_grenade()) {
		if (game::main.get_local()->m_hActiveWeapon().Get()->is_grenade() && game::main.get_local()->m_hActiveWeapon().Get()->m_fThrowTime() > 0.f)
			return;
	}
	else {
		if (game::main.get_local()->m_hActiveWeapon().Get()->GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE && ((game::main.get_command()->buttons & IN_ATTACK) || (game::main.get_command()->buttons & IN_ATTACK2))) {
			return;
		}
		else if ((game::main.get_command()->buttons & IN_ATTACK) && (game::main.get_local()->m_hActiveWeapon().Get()->m_iItemDefinitionIndex() != WEAPON_C4)) {
			if (game::main.get_local()->m_hActiveWeapon().Get()->can_fire())
				return;
		}
	}

	if (game::main.get_local()->m_MoveType() == MOVETYPE_NOCLIP || game::main.get_local()->m_MoveType() == MOVETYPE_LADDER)
		return;
	
	if (game::main.send_packet)
		need_to_flip = !need_to_flip;

	switch (c_config::get().pitch) {
		case 1: cmd->viewangles.x = 89.9f; break;
		case 2: cmd->viewangles.x = 0; break;
		case 3: cmd->viewangles.x = -89.9f; break;
	}

	switch (c_config::get().yaw)
	{
	case 1:
		cmd->viewangles.y += 180.f;
		need_to_flip ? cmd->viewangles.y -= c_config::get().jitter : cmd->viewangles.y += c_config::get().jitter;

		break;
	}

	bool val;
	bool real_val;

	if (c_config::get().invert_fake)
		val = utils::method_key_bind(c_config::get().hold_invert, c_config::get().invert_key);
	else
		val = false;

	if (c_config::get().invert_real)
		real_val = utils::method_key_bind(c_config::get().hold_invert, c_config::get().invert_key);
	else
		real_val = false;

	auto normal_val = c_config::get().fake_mode == 3 ? 60.f : 120.f;

	float amount_left = normal_val * (c_config::get().fake_left / 100.f);
	float amount_right = normal_val * (c_config::get().fake_right / 100.f);

	float body_lean = 0.f;

	if (real_val)
		body_lean = 59 - (c_config::get().body_lean * 0.59);
	else
		body_lean = 59 - (c_config::get().inverted_body_lean * 0.59);
	
	if (real_val)
		cmd->viewangles.y += 45 - body_lean;
	else
		cmd->viewangles.y -= 45 - body_lean;
	
	if (!game::main.fake_lagging)
		game::main.send_packet = cmd->command_number % 3;

	switch (c_config::get().fake_mode)
	{
	case 1:

		if (!game::main.send_packet)
		{
			if (val)
				cmd->viewangles.y = cmd->viewangles.y + amount_left;
			else
				cmd->viewangles.y = cmd->viewangles.y - amount_right;
		}

		if (fabsf(cmd->sidemove) < 5.0f) {
			if (cmd->buttons & IN_DUCK)
				cmd->sidemove = interfaces::global_vars->tickcount & 1 ? 3.25f : -3.25f;
			else
				cmd->sidemove = interfaces::global_vars->tickcount & 1 ? 1.1f : -1.1f;
		}

		break;

	case 2:

		if (!game::main.send_packet)
		{
			if (val)
				cmd->viewangles.y = cmd->viewangles.y + amount_left;
			else
				cmd->viewangles.y = cmd->viewangles.y - amount_right;
		}

		break;

	case 3:

		if (!game::main.send_packet)
		{
			if (val)
				cmd->viewangles.y = cmd->viewangles.y + amount_left;
			else
				cmd->viewangles.y = cmd->viewangles.y - amount_right;
		}

		if (next_lby_update(0.f, cmd))
			cmd->viewangles.y -= 180.f;

		break;	
	}

	if (c_config::get().leg)
		cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);

}

void anti_aim::fake_lag_limit()
{
	static auto clMoveChokeClamp = (uintptr_t)utils::find_signature("engine.dll", "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC") + 1;

	unsigned long protect = 0;

	VirtualProtect((void*)clMoveChokeClamp, 4, PAGE_EXECUTE_READWRITE, &protect);
	*(std::uint32_t*)clMoveChokeClamp = 62;
	VirtualProtect((void*)clMoveChokeClamp, 4, protect, &protect);
}

void anti_aim::fake_lag_start()
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (!game::main.get_local()->is_alive())
		return;

	if (!game::main.get_local()->m_hActiveWeapon().Get())
		return;

	if (game::main.get_local()->m_hActiveWeapon().Get()->is_grenade())
		return;

	if (interfaces::engine->IsVoiceRecording())
		return;

	//if (game::main.get_command()->buttons & IN_ATTACK)
		//return;

	if (!c_config::get().fake_lag_start)
		return;

	if (c_config::get().fakelag_mode == 0)
		return;

	int to_choke;
	auto TIME_TICK = ((int)(1.0f / interfaces::global_vars->interval_per_tick)) / 64;

	switch (c_config::get().fakelag_mode)
	{

	case 1:

		to_choke = std::min(c_config::get().lag_limit * TIME_TICK, 14);

		break;

	case 2:
		
		to_choke = math::clamp(static_cast<int>(std::ceilf(69 / (game::main.get_local()->m_vecVelocity().Length() * interfaces::global_vars->interval_per_tick))), c_config::get().lag_limit, 14);
	
		break;
	}

	static int choked = 0;

	if (choked > to_choke) {
		game::main.send_packet = true;
		choked = 0;
	}
	else {
		game::main.send_packet = false;
		choked++;
	}

	game::main.fake_lagging = true;

}