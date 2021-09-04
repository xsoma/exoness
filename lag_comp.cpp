#include "lag_comp.h"
#include "mathematics.h"
#include <algorithm>

float lag_comp::lerp_time()
{
	int updaterate = interfaces::cvar->FindVar("cl_updaterate")->GetInt();
	ConVar* minupdate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = interfaces::cvar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = interfaces::cvar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = math::clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return std::max(lerp, (ratio / updaterate));
}

bool lag_comp::valid_tick(int tick)
{
	auto nci = interfaces::engine->GetNetChannelInfo();

	if (!nci)
		return false;

	auto PredictedCmdArrivalTick = game::main.get_command()->tick_count + 1 + time_to_ticks(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
	auto Correct = math::clamp(lerp_time() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - ticks_to_time(PredictedCmdArrivalTick + time_to_ticks(lerp_time()) - (tick + time_to_ticks(lerp_time())));

	return (abs(Correct) <= 0.2f);
}

void lag_comp::store_record(c_base_entity* pEnt)
{
	PlayerRecords Setup;

	static float ShotTime[65];
	static float OldSimtime[65];

	pEnt->invalidate_bone_cache();

	if (PlayerRecord[pEnt->EntIndex()].size() == 0)
	{
		Setup.state = pEnt->get_animation_state();

		Setup.SimTime = pEnt->m_flSimulationTime();

		Setup.Shot = false;

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);
	}

	if (OldSimtime[pEnt->EntIndex()] != pEnt->m_flSimulationTime())
	{
		Setup.state = pEnt->get_animation_state();

		Setup.SimTime = pEnt->m_flSimulationTime();

		if (pEnt->m_hActiveWeapon().Get() && !pEnt->m_hActiveWeapon().Get()->is_non_aim())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->m_hActiveWeapon().Get()->m_fLastShotTime())
			{
				Setup.Shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->m_hActiveWeapon().Get()->m_fLastShotTime();
			}
			else
				Setup.Shot = false;
		}
		else
		{
			Setup.Shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);

		OldSimtime[pEnt->EntIndex()] = pEnt->m_flSimulationTime();
	}

	ShotTick[pEnt->EntIndex()] = -1;

	if (PlayerRecord[pEnt->EntIndex()].size() > 0)
		for (int tick = 0; tick < PlayerRecord[pEnt->EntIndex()].size(); tick++)
			if (!valid_tick(time_to_ticks(PlayerRecord[pEnt->EntIndex()].at(tick).SimTime)))
				PlayerRecord[pEnt->EntIndex()].erase(PlayerRecord[pEnt->EntIndex()].begin() + tick);
			else if (PlayerRecord[pEnt->EntIndex()].at(tick).Shot)
				ShotTick[pEnt->EntIndex()] = tick;
}

void lag_comp::clear(int i)
{
	if (PlayerRecord[i].size() > 0)
	{
		for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
		{
			PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
		}
	}
}

PlayerRecords lag_comp::get_record(c_base_entity* e)
{
	auto index = e->EntIndex();

	if (PlayerRecord[index].size() > 0)
		return this->PlayerRecord[index].at(PlayerRecord[index].size() - 1);
}