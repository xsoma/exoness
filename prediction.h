#pragma once
#include "csgo_hooks.h"

struct DataRecord
{
	Vector m_aimPunchAngle = { };
};

class c_prediction : public singleton <c_prediction>
{

public:

	void run_prediction(CUserCmd* cmd);
	void finish();

	Vector unpredicted_eye_pos = {};
	float in_curtime;

public:

	void store_data();
	void update_data(ClientFrameStage_t stage);
	DataRecord m_data_history[128] = { };
};