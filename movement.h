#pragma once
#include "csgo_hooks.h"

class m_movement : public singleton <m_movement>
{
public:

	void run_bhop(CUserCmd* cmd);
	void run_strafe(CUserCmd* cmd, int strafe_type);
	void run_slow_walk(CUserCmd* get_cmd, float get_speed);

};