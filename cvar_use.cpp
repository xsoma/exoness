#include "cvar_use.h"
#include "menu.h"

void cvar::on_create_move ( ) 
{
	if ( !interfaces::engine->IsConnected( ) || !interfaces::engine->IsInGame( ) )
		return;

	if (c_config::get().ambient != 0)
		interfaces::cvar->FindVar("r_modelAmbientMin")->SetValue(c_config::get().ambient);
}

void cvar::on_fsn()
{
	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected() || !game::main.get_local())
		return;

	static bool init = false;
	static bool set_angle = false;
	static int stored_thirdperson_distance;

	auto value = utils::method_key_bind(c_config::get().hold_tp, c_config::get().tp_key);

	if (stored_thirdperson_distance != c_config::get().thirdperson_distance) {
		std::string command; command += "cam_idealdist "; command += std::to_string(c_config::get().thirdperson_distance + 30);
		interfaces::engine->ExecuteClientCmd(command.c_str());

		stored_thirdperson_distance = c_config::get().thirdperson_distance;
	}

	if (value && game::main.get_local()->is_alive())
	{
		if (init)
		{
			ConVar* sv_cheats = interfaces::cvar->FindVar("sv_cheats");
			*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0;
			sv_cheats->SetValue(1);
			interfaces::engine->ExecuteClientCmd("thirdperson");

			std::string command; command += "cam_idealdist "; command += std::to_string(c_config::get().thirdperson_distance + 30);
			interfaces::engine->ExecuteClientCmd(command.c_str());
		}
		init = false;
	}
	else
	{
		if (!init)
		{
			ConVar* sv_cheats = interfaces::cvar->FindVar("sv_cheats");
			*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0;
			sv_cheats->SetValue(1);
			interfaces::engine->ExecuteClientCmd("firstperson");
		}
		init = true;
	}
}