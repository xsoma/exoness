#pragma once
#include "csgo_hooks.h"
#include "main_entity.h"

struct player_animation_t
{
	float m_last_simtime = 0.f;
	float m_spawntime = 0.f;
	bool m_init = false;
	std::array< AnimationLayer, 13 > m_layers{ };
	std::array< float, 20 > m_poses{ };
	CCSGOPlayerAnimState* m_state = nullptr;
	bool m_got_matrix = false;
	matrix3x4_t m_matrix[128]{};
	bool m_should_recalculate_matrix = false;
};

class c_animations : public singleton <c_animations>
{
public:

	void manage_fake_animations();
	void manage_real_animations();
	void player_animations();
	void rebuild(c_base_entity* e);
	void final_fix(c_base_entity* e);

public: // fake

	bool m_should_update_fake = false;
	std::array< AnimationLayer, 13 > m_fake_layers;
	std::array< float, 20 > m_fake_poses;
	CCSGOPlayerAnimState* m_fake_state = nullptr;
	float m_fake_rotation = 0.f;
	bool init_fake_anim = false;
	float m_fake_spawntime = 0.f;
	float m_fake_delta = 0.f;
	matrix3x4_t m_fake_matrix[128];
	matrix3x4_t m_fake_position_matrix[128];
	bool m_got_fake_matrix = false;
	float m_real_yaw_ang = 0.f;


public: // real

	bool m_should_update_real = false;
	float m_real_spawntime = 0.f;
	bool init_real_anim = false;
	std::array< AnimationLayer, 13 > m_real_layers;
	std::array< float, 20 > m_real_poses;
	CCSGOPlayerAnimState* m_real_state = nullptr;
	bool m_got_real_matrix = false;
	float m_real_goalyaw = 0.f;
	matrix3x4_t m_real_matrix[128];
	matrix3x4_t m_real_position_matrix[128];
	float m_server_abs_rotation = 0.f;

public: // animations main

	bool m_should_update_entity_animstate = true;
	std::array< player_animation_t, 64 > m_player_states;
};