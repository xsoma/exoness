#pragma once
#include "i_interfaces.h"
#include "main_entity.h"
#include <deque>

struct PlayerRecords
{
	float SimTime = -1.f;
	Vector Head;
	bool Shot;
	float Velocity;
	matrix3x4_t Matrix[128];
	float Pitch;
	float m_iTickCount;
	CCSGOPlayerAnimState* state;
};

class lag_comp : public singleton <lag_comp>
{
public:

	float lerp_time();
	bool valid_tick(int tick);
	void store_record(c_base_entity* pEnt);
	void clear(int i);

	float ShotTime[65];
	int ShotTick[65];

	std::deque<PlayerRecords> PlayerRecord[65] = {  };

	PlayerRecords get_record(c_base_entity* e);

};
