#pragma once
#include "main_entity.h"
#include "e_nums.h"

struct fire_bullet_data_t {
	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class c_autowall :public singleton <c_autowall>
{
public:

	bool handle_bullet_penetration(CCSWeaponInfo* weapon_data, trace_t& enter_trace, Vector& eye_position, Vector direction, int& possible_hits_remaining, float& current_damage, float penetration_power, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration, fire_bullet_data_t& data);
	bool classname_is(c_base_entity* entity, const char* class_name);
	bool is_breakable(c_base_entity* entity);
	void get_bullet_type(float& max_range, float& max_distance, char* bullet_type, bool sv_penetration_type);
	void clip_trace_to_players(const Vector& start, const Vector& end, std::uint32_t mask, CTraceFilter* filter, trace_t* trace);
	void scale_dmg(c_base_entity* entity, CCSWeaponInfo* weapon_info, int hitgroup, float& current_damage);
	bool fire_bullet(c_base_combat_weapon* pWeapon, Vector& direction, float& current_damage, fire_bullet_data_t& data);
	bool did_hit_non_world_entity(c_base_entity* entity);
	bool trace_to_exit_short(Vector& point, Vector& dir, const float step_size, float max_distance);
	float get_thickness(Vector& start, Vector& end);
	bool trace_to_exit(trace_t& enter_trace, trace_t& exit_trace, Vector start_position, Vector direction);
	bool is_armored(c_base_entity* player, int armor, int hitgroup);
	float get_damage_vec(Vector point);
	void trace_line(Vector& vec_abs_start, Vector& vec_abs_end, unsigned int mask, c_base_entity* ignore, trace_t* ptr);

private:
};
