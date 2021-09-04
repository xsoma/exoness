#pragma once
#include "main_entity.h"

class anti_aim : public singleton <anti_aim>
{
public:

	void on_create_move(CUserCmd* cmd);
	void fake_lag_start();
	void fake_lag_limit();

};