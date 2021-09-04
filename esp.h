#pragma once

#include "main_entity.h"

struct entity_box_info_t
{
	int x, y, w, h, alpha, alpha_anim;

	c_base_entity* ent;
};

struct bottom_info_t
{
	bool isString;
	std::string str;
	double maxValue;
	double percentage;
	Color color;
};

class molotov_info
{
public:
	molotov_info(Vector positions, bool enableds, int entityids)
	{
		this->position = positions;
		this->enabled = enableds;
		this->entityid = entityids;
	}

	Vector position;
	bool   enabled;
	int    entityid;
};

class esp : public singleton <esp>
{
public:

	void on_render();
	void on_render_scope();
	void on_render_molotov();

	std::vector<molotov_info> molotov_logs;
	std::vector<molotov_info> smoke_logs;

private:

	struct
	{
		int left, top, right, bottom;
	}bbox;

	entity_box_info_t infos[4096];

	void bound_box(c_base_entity* pEnt);
	void render_box(c_base_entity* pEnt);
	void on_render_weapon_name(c_base_entity* pEnt);
	void on_render_ammo(c_base_entity* Entity);
	void on_render_name(c_base_entity* pEnt, int iterator);
	void on_render_health(c_base_entity* pEnt);
	void render_flags(c_base_entity* ent);
	void oof_arrows(c_base_entity* player);
	void rotate(std::array< Vector2D, 3 >& points, float rotation);

private:

	bool bound_box_weapon(c_base_entity* m_entity, entity_box_info_t& box, bool dynamic);
	void draw_box_weapon(entity_box_info_t inf);
	void draw_name_weapon(entity_box_info_t inf);
	void draw_ammo_weapon(entity_box_info_t inf);

};
