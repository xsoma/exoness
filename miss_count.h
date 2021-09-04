#pragma once
#include "main_entity.h"
#include <deque>

struct event_data_t
{
	Vector pos = Vector(0, 0, 0);
	bool got_pos = false;
	bool got_hurt = false;
	float time = 0.f;
	int hurt_player = -1;
	int userid = -1;
	bool hurt = false;
	int damage = 0;
	bool died = false;
	int hitbox = HITBOX_MAX;
};

class miss_count : public singleton <miss_count> {

public:

	matrix3x4_t matrixed[128];

	std::deque< event_data_t > last_events;

	int last_rbot_entity = -1;
	float last_rbot_shot_time = 0.f;

	void setup_event(IGameEvent* event);
	bool valid_to_setup_record(c_base_entity* x);
	void set_data(c_base_entity* player, int index, Vector angle);

	bool visible_bone(c_base_entity* Target, int Bone);
	bool player_visible(c_base_entity* target);

	void build_();

	Vector last_rbot_shot_eyepos = Vector(0, 0, 0);
	Vector last_rbot_shot_angle = Vector(0, 0, 0);

	void create_move();
	std::deque<Vector> dequeBulletImpacts;
	bool bPlayerHurt[64];
	bool bBulletImpact[64];
	bool fire[64];

};