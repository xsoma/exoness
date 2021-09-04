#include "csgo_hooks.h"
#include "menu.h"
#include "main_entity.h"

typedef void(__fastcall* over_ride_view_t) (void*, void*, CViewSetup*);

void __fastcall hooks::over_ride_view(void* ecx, void* edx, CViewSetup* pSetup)
{
	static auto oOverrideView = hooks::client_mode_hook->get_original<over_ride_view_t>(18);

	if (interfaces::engine->IsConnected() && interfaces::engine->IsInGame())
	{
		if (!game::main.get_local())
			return;

		if (!game::main.get_local()->is_alive())
			return;

		if (c_config::get().remove[3])
		{
			Vector viewPunch = game::main.get_local()->m_viewPunchAngle();
			Vector aimPunch = game::main.get_local()->m_aimPunchAngle();

			pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
			pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
			pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		}

		if (c_config::get().fov != 0 && !game::main.get_local()->m_bIsScoped())
			pSetup->fov = c_config::get().fov;

		if (c_config::get().remove_zoom && game::main.get_local()->m_bIsScoped())
			pSetup->fov = (c_config::get().fov == 0) ? 90 : c_config::get().fov;
	}

	oOverrideView(ecx, edx, pSetup);
}