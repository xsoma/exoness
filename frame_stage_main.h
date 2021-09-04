#pragma once
#include "globals.h"

enum resolve_process
{
	RESOLVE_NONE,
	RESOLVE_START,
	RESOLVE_SIDE_OPPOSITE,
	RESOLVE_SIDE_1,
	RESOLVE_SIDE_2
};

enum resolve_aspects
{
	ASPECT_NONE,
	ASPECT_BREAKER
};

struct resolve_data
{

	bool fire;
	int miss;
	int abs_miss;
	int r_mode;
	int aspects;
	CCSGOPlayerAnimState* state;

	void reset()
	{
		miss = 0;
		r_mode = 0;
		aspects = 0;
	}

};

class frame : public singleton <frame>
{
public:

	void on_frame_stage_notify(ClientFrameStage_t stage);

	resolve_data infos[65];

	float server_feet_yaw(c_base_entity* e);
	bool is_animation_triggers(c_base_entity* e);

	void update_info(c_base_entity* e);
	void main_part(c_base_entity* e);
	void resolve(c_base_entity* e);

	AnimationLayer previous_layer = { };

};