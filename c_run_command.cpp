#include "csgo_hooks.h"
#include "prediction.h"
#include "globals.h"

using fn = void(__thiscall*)(void*, c_base_entity*, CUserCmd*, IMoveHelper*);

void __fastcall hooks::run_cmd(void* ecx, void*, c_base_entity * player, CUserCmd* ucmd, IMoveHelper* helper)
{
    static auto original = hooks::prediction_hook->get_original< fn >(19);

	if (!game::main.get_local()->is_alive() || !interfaces::engine->IsInGame())
	{
		original(ecx, player, ucmd, helper);
		return;
	}

	original(ecx, player, ucmd, helper);

    c_prediction::get().store_data();
}