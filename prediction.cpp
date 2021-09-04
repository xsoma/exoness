#include "prediction.h"
#include "main_entity.h"
#include "utils.h"

float flOldCurtime;
float flOldFrametime;
int* m_nPredictionRandomSeed;
int* m_pSetPredictionPlayer;
static bool bInit = false;

void c_prediction::run_prediction(CUserCmd* cmd)
{
	if (!game::main.get_local()->is_alive())
		return;

	if (!m_nPredictionRandomSeed || !m_pSetPredictionPlayer) {
		m_nPredictionRandomSeed = *reinterpret_cast<int**>(utils::find_signature("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
		m_pSetPredictionPlayer = *reinterpret_cast<int**>(utils::find_signature("client.dll", "89 35 ? ? ? ? F3 0F 10 46") + 2);
	}

	CMoveData data;
	memset(&data, 0, sizeof(CMoveData));

	this->unpredicted_eye_pos = game::main.get_local()->get_eye_pos();

	game::main.get_local()->current_command() = cmd;
	interfaces::movehelper->SetHost(game::main.get_local());

	*m_nPredictionRandomSeed = cmd->random_seed;
	*m_pSetPredictionPlayer = uintptr_t(game::main.get_local());

	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(game::main.get_local()) + 0x3314) = reinterpret_cast<uint32_t>(cmd);
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(game::main.get_local()) + 0x326C) = reinterpret_cast<uint32_t>(cmd);

	flOldCurtime = interfaces::global_vars->curtime;
	flOldFrametime = interfaces::global_vars->frametime;

	interfaces::global_vars->curtime = utils::get_estimate_server_time(cmd);
	interfaces::global_vars->frametime = interfaces::global_vars->interval_per_tick;

	interfaces::gamemovement->StartTrackPredictionErrors(game::main.get_local());

	interfaces::prediction->SetupMove(game::main.get_local(), cmd, interfaces::movehelper, &data);
	interfaces::gamemovement->ProcessMovement(game::main.get_local(), &data);
	interfaces::prediction->FinishMove(game::main.get_local(), cmd, &data);

	if (game::main.get_local()->m_hActiveWeapon().Get())
		game::main.get_local()->m_hActiveWeapon().Get()->penalty();

	// this->store_data();
}

void c_prediction::finish()
{
	if (!game::main.get_local()->is_alive() || !interfaces::movehelper)
		return;

	interfaces::gamemovement->FinishTrackPredictionErrors(game::main.get_local());
	interfaces::movehelper->SetHost(nullptr);

	if (m_nPredictionRandomSeed || m_pSetPredictionPlayer)
	{
		*m_nPredictionRandomSeed = -1;
		*m_pSetPredictionPlayer = 0;
	}

	interfaces::global_vars->curtime = flOldCurtime;
	interfaces::global_vars->frametime = flOldFrametime;

	game::main.get_local()->current_command() = NULL;
}

void c_prediction::store_data()
{
	auto player = game::main.get_local();

	if (player && player->is_alive())
	{
		auto command = player->m_nTickBase();
		auto data = &m_data_history[command % 63];

		data->m_aimPunchAngle = player->m_aimPunchAngle();
	}
}

void c_prediction::update_data(ClientFrameStage_t stage)
{
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		auto player = game::main.get_local();

		if (player && player->is_alive())
		{
			auto command = player->m_nTickBase();
			auto data = &m_data_history[command % 63];

			player->m_aimPunchAngle() = data->m_aimPunchAngle;
		}
	}
}