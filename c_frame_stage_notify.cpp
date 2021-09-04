#include "csgo_hooks.h"
#include "menu.h"
#include "main_entity.h"
#include "world.h"
#include "animations.h"
#include "cvar_use.h"
#include "frame_stage_main.h"
#include "prediction.h"

using frame_stage_notify_t = void(__stdcall*)(ClientFrameStage_t);

void __stdcall hooks::frame_stage_notify(ClientFrameStage_t stage) {

	static auto original_fn = hooks::client_hook->get_original<frame_stage_notify_t>(37);

	world::get().on_fsn_state(stage);

	world::get().on_paint_traverse();

	cvar::get().on_fsn();

	frame::get().on_frame_stage_notify(stage);

	Vector anglez = Vector(game::main.real_angle.x, game::main.real_angle.y, 0);

	// c_prediction::get().update_data(stage);

	if (stage == FRAME_RENDER_START)
	{
		if (interfaces::engine->IsInGame() || interfaces::engine->IsConnected())
		{
			*(bool*)interfaces::postprocessing = c_config::get().remove[4] ? true : false;

			if (game::main.get_local() && c_config::get().remove[1])
				game::main.get_local()->m_flFlashDuration() = 0.0;

			if (game::main.get_local() && game::main.get_local()->is_alive() && interfaces::input->m_fCameraInThirdPerson)
				interfaces::prediction->SetLocalAngles(anglez);

			if (game::main.get_local() && game::main.get_local()->is_alive())
			{
				c_animations::get().manage_real_animations();
			}
		}
	}

	if (stage == FRAME_NET_UPDATE_END)
	{
		if (interfaces::engine->IsInGame() || interfaces::engine->IsConnected())
		{
			//if (game::main.get_local())
				//c_animations::get().player_animations();
		}
	}

	original_fn(stage);
}