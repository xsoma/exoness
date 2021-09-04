#pragma once
#include "i_interfaces.h"
#include <vector>

struct target_t {
	float best_distance = 1000000.0f;
	int tid = -1;
	Vector aimspot;
	float simtime = 0.f;
	void reset() {
		best_distance = INT_MAX;
		tid = -1;
		simtime = 0.f;
		aimspot.Init();
	}
};

class c_rage : public singleton <c_rage>
{
public:

	void on_create_move(CUserCmd* m_pcmd);

	int target_id;
	Vector engine_angles;

	void scan();
	bool hitchance(Vector angle, c_base_entity* e, float chance);
	void update();
	void get_target(c_base_entity* e, float fov, Vector location, float simulation_time);
	Vector best_point(c_base_entity* pEnt);
	void aim();
	bool spread_limit();
	void generate_points(c_base_entity* entity, int hitbox_id, std::vector< Vector >& points);
	Vector angle_reverse(c_base_entity* pEntity, Vector& vPunchAngleVelocity);
	bool head_valid(c_base_entity* e);
	void exploits_run();

public:

	struct
	{

		int min_damage;
		int min_hit;
		int head_scale;
		int body_scale;
		int prior_hitbox;

	}config;

private:

	target_t target_data;

};