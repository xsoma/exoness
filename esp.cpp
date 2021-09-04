#include "esp.h"
#include "draw_manager.h"
#include "config.h"
#include <Psapi.h>
#include "utils.h"
#include "mathematics.h"

float DormantAlpha[1024];
float StoredCurtimePlayer[1024];

void esp::bound_box(c_base_entity* pEnt)
{
	bbox.bottom = -1;
	bbox.top = -1;
	bbox.left = -1;
	bbox.right = -1;

	Vector2D w2sBottom, w2sTop;

	utils::world_to_screen(pEnt->m_vecOrigin() - Vector(0, 0, 8), w2sBottom);
	utils::world_to_screen(pEnt->hitbox_position(0) + Vector(0, 0, 10), w2sTop);

	int Middle = w2sBottom.y - w2sTop.y;
	int Width = Middle / 3.55f;

	bbox.bottom = Middle;
	bbox.top = w2sTop.y;
	bbox.left = w2sBottom.x - Width;
	bbox.right = Width * 2;
}

void esp::on_render()
{
	for (int i = 1; i < interfaces::entitylist->GetHighestEntityIndex(); ++i)
	{
		c_base_entity* pPlayerEntity = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(i));

		if (!pPlayerEntity)
			continue;

		auto mdl = interfaces::modelinfo->GetModelName(pPlayerEntity->GetModel());

		if (pPlayerEntity->GetClientClass()->m_ClassID == ClassId::ClassId_CCSPlayer && c_config::get().visuals_enabled)
		{
			if (!pPlayerEntity
				|| !pPlayerEntity->is_alive()
				|| pPlayerEntity == game::main.get_local()
				|| pPlayerEntity->m_iTeamNum() == game::main.get_local()->m_iTeamNum()
				)
				continue;

			if (!pPlayerEntity->IsDormant()) {
				StoredCurtimePlayer[pPlayerEntity->EntIndex()] = interfaces::global_vars->curtime;
			}

			if (pPlayerEntity->IsDormant() && DormantAlpha[pPlayerEntity->EntIndex()] > 0 && interfaces::global_vars->curtime - StoredCurtimePlayer[pPlayerEntity->EntIndex()] > 2)
			{
				DormantAlpha[pPlayerEntity->EntIndex()] -= 5;
			}
			else if (DormantAlpha[pPlayerEntity->EntIndex()] < 255 && !(pPlayerEntity->IsDormant()))
			{
				DormantAlpha[pPlayerEntity->EntIndex()] += 5;
			}

			this->bound_box(pPlayerEntity);

			if (bbox.bottom == 0)
				continue;

			if (c_config::get().draw_box)
				this->render_box(pPlayerEntity);

			if (c_config::get().draw_health)
				this->on_render_health(pPlayerEntity);

			if (c_config::get().draw_name)
				this->on_render_name(pPlayerEntity, i);

			if (c_config::get().draw_ammo)
				this->on_render_ammo(pPlayerEntity);

			if (c_config::get().draw_weapon)
				this->on_render_weapon_name(pPlayerEntity);

			if (c_config::get().fov_arrows_enemy)
				this->oof_arrows(pPlayerEntity);

			if (c_config::get().flags[0] || c_config::get().flags[1] || c_config::get().flags[2])
				this->render_flags(pPlayerEntity);

		}
		else if (pPlayerEntity->IsWeapon() && c_config::get().visuals_world_enabled)
		{
			entity_box_info_t& box = infos[i - 1];

			if (!strstr(mdl, "w_"))
				continue;

			if (bound_box_weapon(pPlayerEntity, box, true))
			{
				if (c_config::get().weapon_draw_name)
					this->draw_name_weapon(box);

				if (c_config::get().weapon_draw_box)
					this->draw_box_weapon(box);

				if (c_config::get().weapon_draw_ammo)
					this->draw_ammo_weapon(box);
			}
		}
		
	}
}

void esp::render_box(c_base_entity* Entity)
{
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()]) : Color(c_config::get().box_r, c_config::get().box_g, c_config::get().box_b, DormantAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85) : Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85);

	render::get().outlined_rect(bbox.left, bbox.top, bbox.right, bbox.bottom, ESP);
	render::get().outlined_rect(bbox.left + 1, bbox.top + 1, bbox.right - 2, bbox.bottom - 2, ESP2);
	render::get().outlined_rect(bbox.left - 1, bbox.top - 1, bbox.right + 2, bbox.bottom + 2, ESP2);
}

void esp::on_render_health(c_base_entity* Entity)
{
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()]) : Color(c_config::get().hp_r, c_config::get().hp_g, c_config::get().hp_b, DormantAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85) : Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85);

	render::get().filled_rect(bbox.left - 7, bbox.top - 1, 4, bbox.bottom + 2, ESP2);
	int pixelValue = Entity->m_iHealth() * bbox.bottom / 100;
	render::get().filled_rect(bbox.left - 6, bbox.top + bbox.bottom - pixelValue, 2, pixelValue, ESP);

	std::string Health = std::to_string(Entity->m_iHealth());

	if (Entity->m_iHealth() < 100)
		render::get().draw_text(bbox.left - 9, bbox.top + bbox.bottom - pixelValue - 5, Color(255, 255, 255, DormantAlpha[Entity->EntIndex()] * 0.85), render::get().visuals_font, false, Health.c_str());

}

void esp::on_render_name(c_base_entity* pEnt, int i)
{
	player_info_t pInfo;
	interfaces::engine->GetPlayerInfo(i, &pInfo);
	Color ESP = pEnt->IsDormant() ? Color(150, 150, 150, DormantAlpha[pEnt->EntIndex()]) : Color(c_config::get().name_r, c_config::get().name_g, c_config::get().name_b, DormantAlpha[pEnt->EntIndex()]);
	std::string name = pInfo.szName;

	wchar_t buffer[128];
	wsprintfW(buffer, L"%S", pInfo.szName);

	if (MultiByteToWideChar(CP_UTF8, 0, pInfo.szName, -1, buffer, 128) > 0)
		render::get().draw_text(bbox.left + (bbox.right / 2), bbox.top - 13, ESP, render::get().no_shadow, true, name.c_str());
	else
		render::get().draw_text(bbox.left + (bbox.right / 2), bbox.top - 13, ESP, render::get().visuals_font_name, true, name.c_str());
}

void esp::on_render_weapon_name(c_base_entity* Entity)
{
	Color weapon = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()] * 0.9) : Color(c_config::get().weapon_r, c_config::get().weapon_g, c_config::get().weapon_b, DormantAlpha[Entity->EntIndex()] * 0.9);

	auto weapon_info = Entity->m_hActiveWeapon().Get();
	int correct = 0;

	if (c_config::get().draw_ammo)
		correct = 9;
	else
		correct = 3;

	if (!weapon_info)
		return;

	std::string name = weapon_info->GetCSWeaponData()->szHudName;
	name.erase(0, 13);
	std::transform(name.begin(), name.end(), name.begin(), toupper);

	render::get().draw_text(bbox.left + (bbox.right / 2), bbox.top + bbox.bottom + correct, weapon, render::get().visuals_font, true, name.c_str());
}

void esp::on_render_ammo(c_base_entity* Entity)
{
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()]) : Color(c_config::get().ammo_r, c_config::get().ammo_g, c_config::get().ammo_b, DormantAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85) : Color(0, 0, 0, DormantAlpha[Entity->EntIndex()] * 0.85);

	auto weapon = Entity->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	int ammo = weapon->m_iClip1();

	float
		bar_width = ammo * bbox.right / weapon->GetCSWeaponData()->iMaxClip1;

	AnimationLayer animlayer = Entity->get_animlayers()[1];

	if (animlayer.m_pOwner) {

		auto activity = Entity->sequence_activity(animlayer.m_nSequence);

		bool reloading = (activity == 967 && animlayer.m_flWeight != 0.0f);

		if (reloading && animlayer.m_flCycle < 0.99)
			bar_width = (animlayer.m_flCycle * bbox.right) / 1.f;
	}

	render::get().filled_rect(bbox.left - 1, bbox.top + bbox.bottom + 3, bbox.right + 2, 4, ESP2);
	render::get().filled_rect(bbox.left, bbox.top + bbox.bottom + 4, bar_width, 2, ESP);
}

void esp::on_render_scope()
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (!game::main.get_local())
		return;

	int w, h = 0;
	interfaces::engine->GetScreenSize(w, h);

	if (game::main.get_local()->m_bIsScoped())
	{
		render::get().line(w / 2, 0, w / 2, h, Color(0, 0, 0, 255));
		render::get().line(0, h / 2, w, h / 2, Color(0, 0, 0, 255));
	}
}

bool esp::bound_box_weapon(c_base_entity* m_entity, entity_box_info_t& box, bool dynamic)
{
	if (!m_entity)
		return false;

	const VMatrix& trans = m_entity->GetCollisionBoundTrans();
	ICollideable* collision = m_entity->GetCollideable();

	if (!collision)
		return false;

	Vector min, max;
	min = collision->OBBMins();
	max = collision->OBBMaxs();

	if (dynamic)
	{
		Vector points[] = { Vector(min.x, min.y, min.z),
				Vector(min.x, max.y, min.z),
				Vector(max.x, max.y, min.z),
				Vector(max.x, min.y, min.z),
				Vector(max.x, max.y, max.z),
				Vector(min.x, max.y, max.z),
				Vector(min.x, min.y, max.z),
				Vector(max.x, min.y, max.z)
		};
		auto vector_transform = [](const Vector in1, const VMatrix& in2)
		{
			auto dot_product = [](const Vector& v1, const float* v2)
			{
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};
			auto out = Vector();
			out[0] = dot_product(in1, in2[0]) + in2[0][3];
			out[1] = dot_product(in1, in2[1]) + in2[1][3];
			out[2] = dot_product(in1, in2[2]) + in2[2][3];
			return out;
		};
		Vector pointsTransformed[8];

		for (int i = 0; i < 8; i++)
			pointsTransformed[i] = vector_transform(points[i], trans);

		Vector pos = m_entity->get_abs_origin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if (!utils::world_to_screen_vector(pointsTransformed[3], flb) || !utils::world_to_screen_vector(pointsTransformed[5], brt)
			|| !utils::world_to_screen_vector(pointsTransformed[0], blb) || !utils::world_to_screen_vector(pointsTransformed[4], frt)
			|| !utils::world_to_screen_vector(pointsTransformed[2], frb) || !utils::world_to_screen_vector(pointsTransformed[1], brb)
			|| !utils::world_to_screen_vector(pointsTransformed[6], blt) || !utils::world_to_screen_vector(pointsTransformed[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for (int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;

			if (top < arr[i].y)
				top = arr[i].y;

			if (right < arr[i].x)
				right = arr[i].x;

			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}

		Vector BotCenter = Vector(right - ((right - left) / 2), top, 0);
		Vector TopCenter = Vector(right - ((right - left) / 2), bottom, 0);
		box.x = left;
		box.y = TopCenter.y;
		box.w = right - left;
		box.h = BotCenter.y - TopCenter.y;
	}

	else
	{
		Vector pos, pos3D, top, top3D;
		pos3D = m_entity->get_abs_origin();
		top3D = pos3D + Vector(0, 0, max.z);

		if (utils::world_to_screen_vector(pos3D, pos) && utils::world_to_screen_vector(top3D, top))
		{
			int height = (pos.y - top.y);
			int width = height / 2;
			box.x = (pos.x - width / 2);
			box.y = top.y;
			box.w = width;
			box.h = height;
		}
	}

	box.ent = m_entity;
	return true;
}

void esp::draw_box_weapon(entity_box_info_t inf)
{
	render::get().outlined_rect(inf.x - 1, inf.y - 1, inf.w + 2, inf.h + 2, Color(0, 0, 0, 255 * 0.85));
	render::get().outlined_rect(inf.x, inf.y, inf.w, inf.h, Color(c_config::get().weapon_box_r, c_config::get().weapon_box_g, c_config::get().weapon_box_b, 255 * 0.85));
	render::get().outlined_rect(inf.x + 1, inf.y + 1, inf.w - 2, inf.h - 2, Color(0, 0, 0, 255 * 0.85));
}

void esp::draw_name_weapon(entity_box_info_t inf)
{
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	std::string name = clean_item_name(inf.ent->GetClientClass()->m_pNetworkName);
	std::transform(name.begin(), name.end(), name.begin(), toupper);

	int textWidth = render::get().get_text_size_rect(render::get().visuals_font, name.c_str()).right;

	int x = inf.x;
	render::get().draw_text(inf.x + (inf.w / 2) - (textWidth / 2), inf.y - 13, Color(c_config::get().weapon_name_r, c_config::get().weapon_name_g, c_config::get().weapon_name_b, 255 * 0.85), render::get().visuals_font, false, name.c_str());
}

void esp::draw_ammo_weapon(entity_box_info_t inf)
{
	if (inf.ent->GetClientClass()->m_ClassID == ClassId::ClassId_CC4)
		return;

	if (inf.ent->GetClientClass()->m_ClassID == ClassId::ClassId_CBaseCSGrenadeProjectile)
		return;

	if (inf.ent->GetClientClass()->m_ClassID == ClassId::ClassId_CWeaponTaser)
		return;

	std::vector<bottom_info_t> botInfos;
	auto weapon = (c_base_combat_weapon*)inf.ent;

	if (weapon)
	{
		int maxAmmo = weapon->GetCSWeaponData()->iMaxClip1;
		int curAmmo = weapon->m_iClip1();
		botInfos.push_back({ false, "", (double)maxAmmo, (double)curAmmo, Color(c_config::get().weapon_ammo_r,c_config::get().weapon_ammo_g,c_config::get().weapon_ammo_b, 255 * 0.85)});
	}

	int yOffset = 0;
	int textHeight = render::get().get_text_size_rect(render::get().visuals_font_name, "A").bottom + 1;

	for (auto i : botInfos)
	{
		render::get().filled_rect(inf.x - 1, inf.y + inf.h + 2 + yOffset, inf.w + 2, 4, Color(0,0,0, 255 * 0.85));
		render::get().filled_rect(inf.x, inf.y + inf.h + 2 + yOffset + 1, ceil((double)inf.w * (i.percentage / i.maxValue)), 2, i.color);
		yOffset += 5;
	}
}

void esp::rotate(std::array< Vector2D, 3 >& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}

void esp::oof_arrows(c_base_entity* player) {
	auto local = game::main.get_local();

	if (!local)
		return;

	if (!local->is_alive())
		return;

	if (!player)
		return;

	if (player->IsDormant())
		return;

	Vector position;

	if (utils::world_to_screen_vector(player->hitbox_position(2), position))
		return;

	Vector viewangles;
	int width, height;

	interfaces::engine->GetViewAngles(viewangles);
	interfaces::engine->GetScreenSize(width, height);

	const auto screen_center = Vector2D(width * .5f, height * .5f);

	const auto angle_yaw_rad = DEG2RAD(viewangles.y - math::calculate_angle(local->get_eye_pos(), player->hitbox_position(2)).y - 90);

	int radius = c_config::get().fov_arrows_enemy_distance;
	int size = c_config::get().fov_arrows_enemy_size;

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	Color ESP = player->IsDormant() ? Color(150, 150, 150, DormantAlpha[player->EntIndex()]) : Color(c_config::get().fov_arrows_esp_color_r, c_config::get().fov_arrows_esp_color_g, c_config::get().fov_arrows_esp_color_b, DormantAlpha[player->EntIndex()]);

	Color clr = ESP;

	std::array< Vector2D, 3 >points{ Vector2D(new_point_x - size, new_point_y - size),
		Vector2D(new_point_x + size, new_point_y),
		Vector2D(new_point_x - size, new_point_y + size) };

	rotate(points, viewangles.y - math::calculate_angle(local->get_eye_pos(), player->hitbox_position(2)).y - 90);
	render::get().draw_filled_triangle(points, clr);
}

void esp::render_flags(c_base_entity* Entity)
{
	Color Zoom = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()] * 0.85) : Color(25, 134, 212, DormantAlpha[Entity->EntIndex()] * 0.85);
	Color HK = Entity->IsDormant() ? Color(150, 150, 150, DormantAlpha[Entity->EntIndex()] * 0.85) : Color(255, 255, 255, DormantAlpha[Entity->EntIndex()] * 0.85);

	std::vector<std::pair<std::string, Color>> flags;

	if (Entity->m_bHasHelmet() && Entity->m_ArmorValue() > 0 && c_config::get().flags[1])
		flags.push_back(std::pair<std::string, Color>("HK", HK));
	else if (!Entity->m_bHasHelmet() && Entity->m_ArmorValue() > 0 && c_config::get().flags[1])
		flags.push_back(std::pair<std::string, Color>("K", HK));

	if (Entity->m_bIsScoped() && c_config::get().flags[0])
		flags.push_back(std::pair<std::string, Color>(std::string("ZOOM"), Zoom));

	int pos = 0;

	for (auto text : flags) {
		render::get().draw_text(bbox.left + bbox.right + 4, bbox.top + pos, text.second, render::get().visuals_font, false, text.first.c_str());
		pos += 9;
	}

}

void esp::on_render_molotov()
{
	for (int i = 0; i < molotov_logs.size(); i++)
	{
		auto pos = molotov_logs[i].position;

		Vector2D in;

		if (utils::world_to_screen(pos, in))
		{
			render::get().circle_3d(pos, 100, 125, Color(255, 0, 0, 255));
			render::get().draw_text(in.x, in.y, Color(255, 255, 255, 255 * 0.85), render::get().visuals_font, true, "MOLLY");
		}
	}

	for (int i = 0; i < smoke_logs.size(); i++)
	{
		auto pos = smoke_logs[i].position;

		Vector2D in;

		if (utils::world_to_screen(pos, in))
		{
			render::get().circle_3d(pos, 100, 125, Color(25, 134, 212, 255));
			render::get().draw_text(in.x, in.y, Color(255, 255, 255, 255 * 0.85), render::get().visuals_font, true, "SMOKE");
		}
	}
}
