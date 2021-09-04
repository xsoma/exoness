#include "csgo_hooks.h"
#include "menu.h"
#include "esp.h"
#include "miss_count.h"
#include "frame_stage_main.h"

void c_hook_events::FireGameEvent(IGameEvent* event)
{
	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected())
		return;

	if (!game::main.get_local())
		return;

	if (!event)
		return;

	miss_count::get().setup_event(event);

	if (!strcmp(event->GetName(), ("round_start")))
		frame::get().infos->abs_miss = 0;

	if (c_config::get().visuals_world_enabled && c_config::get().radius)
	{
		if (!strcmp(event->GetName(), ("inferno_startburn")))
		{
			Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
			esp::get().molotov_logs.emplace_back(molotov_info(position, true, event->GetInt("userid")));
		}

		if (!strcmp(event->GetName(), ("inferno_expire")) || !strcmp(event->GetName(), ("round_start")))
		{
			for (int i = 0; i < esp::get().molotov_logs.size(); i++)
				esp::get().molotov_logs.erase(esp::get().molotov_logs.begin() + i);
		}

		if (!strcmp(event->GetName(), ("smokegrenade_detonate")))
		{
			Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
			esp::get().smoke_logs.emplace_back(molotov_info(position, true, event->GetInt("userid")));
		}

		if (!strcmp(event->GetName(), ("smokegrenade_expired")) || !strcmp(event->GetName(), ("round_start")))
		{
			for (int i = 0; i < esp::get().smoke_logs.size(); i++)
				esp::get().smoke_logs.erase(esp::get().smoke_logs.begin() + i);
		}
	}

	if (c_config::get().hitsound)
	{
		auto attacker = interfaces::entitylist->GetClientEntity(interfaces::engine->GetPlayerForUserID(event->GetInt("attacker")));

		if (!attacker)
			return;

		if (attacker != game::main.get_local())
			return;

		interfaces::engine->ExecuteClientCmd("play buttons\\arena_switch_press_02.wav");
	}
}

int c_hook_events::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void c_hook_events::RegisterSelf()
{
	m_iDebugId = EVENT_DEBUG_ID_INIT;
	interfaces::eventmanager->AddListener(this, "player_hurt", false);
	interfaces::eventmanager->AddListener(this, "item_purchase", false);
	interfaces::eventmanager->AddListener(this, "bomb_beginplant", false);
	interfaces::eventmanager->AddListener(this, "bomb_begindefuse", false);
	interfaces::eventmanager->AddListener(this, "bullet_impact", false);
	interfaces::eventmanager->AddListener(this, "round_prestart", false);
	interfaces::eventmanager->AddListener(this, "round_freeze_end", false);
	interfaces::eventmanager->AddListener(this, "round_start", false);
	interfaces::eventmanager->AddListener(this, "player_death", false);
	interfaces::eventmanager->AddListener(this, "inferno_expire", false);
	interfaces::eventmanager->AddListener(this, "inferno_startburn", false);
	interfaces::eventmanager->AddListener(this, "smokegrenade_detonate", false);
	interfaces::eventmanager->AddListener(this, "smokegrenade_expired", false);
}