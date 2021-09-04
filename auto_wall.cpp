#include "auto_wall.h"
#include "mathematics.h"
#include "fnv_hash.h"
#include "prediction.h"

void c_autowall::clip_trace_to_players(const Vector& start, const Vector& end, std::uint32_t mask, CTraceFilter* filter, trace_t* trace) {
	trace_t player_trace;
	Ray_t ray;
	float smallest_fraction = trace->fraction;
	const float max_range = 60.0f;

	ray.Init(start, end);

	if (!trace->hit_entity) return;
	if (filter && filter->ShouldHitEntity(trace->hit_entity, mask) == false)
		return;

	float range = math::distance_to_ray(((c_base_entity*)(trace->hit_entity))->world_space_center(), start, end);
	if (range >= 0.0f || range <= max_range)
	{
		interfaces::trace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, ((c_base_entity*)(trace->hit_entity)), &player_trace);
		if (player_trace.fraction < smallest_fraction)
		{
			*trace = player_trace;
			smallest_fraction = player_trace.fraction;
		}
	}
};

void c_autowall::scale_dmg(c_base_entity* entity, CCSWeaponInfo* weapon_info, int hitgroup, float& current_damage) {

	bool has_heavy_armor = false;
	if (!entity) return;
	static auto armored = [&](void) {
		switch (hitgroup) {
		case (int)HITGROUP_HEAD: return entity->m_bHasHelmet();
		case (int)HITGROUP_GENERIC:
		case (int)HITGROUP_CHEST:
		case (int)HITGROUP_STOMACH:
		case (int)HITGROUP_LEFTARM:
		case (int)HITGROUP_RIGHTARM: return true;
		default: return false;
		}
	};

	switch (hitgroup) {
	case (int)HITGROUP_HEAD: current_damage *= has_heavy_armor ? 2.0f : 4.0f; break;
	case (int)HITGROUP_STOMACH: current_damage *= 1.25f; break;
	case (int)HITGROUP_LEFTLEG:
	case (int)HITGROUP_RIGHTLEG: current_damage *= 0.75f; break;
	default: break;
	}

	if (entity->m_ArmorValue() > 0 && armored()) {
		auto bonus_val = 1.0f;
		auto armor_bonus_rat = 0.5f;
		auto armor_rat = weapon_info->flArmorRatio / 2.0f;

		if (has_heavy_armor) {
			armor_bonus_rat = 0.33f;
			armor_rat *= 0.5f;
			bonus_val = 0.33f;
		}

		auto dmg = current_damage * armor_rat;

		if (has_heavy_armor)
			dmg *= 0.85f;

		if (((current_damage - (current_damage * armor_rat)) * (bonus_val * armor_bonus_rat)) > entity->m_ArmorValue())
			dmg = current_damage - (entity->m_ArmorValue() / armor_bonus_rat);

		current_damage = dmg;
	}
}

bool c_autowall::fire_bullet(c_base_combat_weapon* pWeapon, Vector& direction, float& current_damage, fire_bullet_data_t& data)
{
	if (!pWeapon)
		return false;

	CCSWeaponInfo* weapon_data = pWeapon->GetCSWeaponData();

	if (!weapon_data)
		return false;

	bool sv_penetration_type;

	float current_distance = 0.f, penetration_power, penetration_distance, max_range, ff_damage_reduction_bullets, ff_damage_bullet_penetration, ray_extension = 40.f;


	static ConVar* penetrationSystem = interfaces::cvar->FindVar(("sv_penetration_type"));
	static ConVar* damageReductionBullets = interfaces::cvar->FindVar(("ff_damage_reduction_bullets"));
	static ConVar* damageBulletPenetration = interfaces::cvar->FindVar(("ff_damage_bullet_penetration"));

	sv_penetration_type = penetrationSystem->GetBool();
	ff_damage_reduction_bullets = damageReductionBullets->GetFloat();
	ff_damage_bullet_penetration = damageBulletPenetration->GetFloat();

	trace_t enter_trace;

	CTraceFilter filter;
	filter.pSkip = game::main.get_local();

	max_range = weapon_data->flRange;

	get_bullet_type(penetration_power, penetration_distance, weapon_data->szHudName, sv_penetration_type);

	if (sv_penetration_type)
		penetration_power = weapon_data->flPenetration;

	int possible_hits_remaining = 4;

	current_damage = weapon_data->iDamage;

	while (possible_hits_remaining > 0 && current_damage >= 1.f)
	{

		max_range -= current_distance;

		Vector end = data.src + direction * max_range;

		trace_line(data.src, end, MASK_SHOT_HULL | CONTENTS_HITBOX, game::main.get_local(), &enter_trace);
		clip_trace_to_players(data.src, end + direction * ray_extension, MASK_SHOT_HULL | CONTENTS_HITBOX, &data.filter, &enter_trace);

		surfacedata_t* enter_surface_data = interfaces::physsurface->GetSurfaceData(enter_trace.surface.surfaceProps);
		float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
		int enter_material = enter_surface_data->game.material;

		if (enter_trace.fraction == 1.f)
			break;

		current_distance += enter_trace.fraction * max_range;
		current_damage *= pow(weapon_data->flRangeModifier, (current_distance / 500.f));

		if (current_distance > penetration_distance&& weapon_data->flPenetration > 0.f || enter_surf_penetration_modifier < 0.1f)
			break;

		bool can_do_damage = (enter_trace.hitgroup != (int)HITGROUP_GENERIC && enter_trace.hitgroup != (int)HITGROUP_GENERIC);

		bool is_enemy = game::main.get_local()->m_iTeamNum() != reinterpret_cast<c_base_entity*>(enter_trace.hit_entity)->m_iTeamNum();

		if ((can_do_damage && is_enemy))
		{
			scale_dmg((c_base_entity*)enter_trace.hit_entity, weapon_data, enter_trace.hitgroup, current_damage);
			return true;
		}

		if (!handle_bullet_penetration(weapon_data, enter_trace, data.src, direction, possible_hits_remaining, current_damage, penetration_power, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration, data))
			break;
	}
	return false;
}

bool c_autowall::did_hit_non_world_entity(c_base_entity* entity)
{
	return entity != nullptr && entity->EntIndex() != 0;
}

bool c_autowall::handle_bullet_penetration(CCSWeaponInfo* weapon_data, trace_t& enter_trace, Vector& eye_position, Vector direction, int& possible_hits_remaining, float& current_damage, float penetration_power, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration, fire_bullet_data_t& data)
{

	if (&current_damage == nullptr)
		throw std::invalid_argument("current damage is null!");

	trace_t exit_trace;
	auto enemy = (c_base_entity*)enter_trace.hit_entity;
	surfacedata_t* enter_surface_data = interfaces::physsurface->GetSurfaceData(enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float enter_damage_modifier = enter_surface_data->game.flDamageModifier;
	float thickness, modifier, lost_damage, final_damage_modifier, combined_penetration_modifier;
	bool is_solid_surface = ((enter_trace.contents >> 3)& CONTENTS_SOLID);
	bool is_light_surface = ((enter_trace.surface.flags >> 7)& SURF_LIGHT);

	if (possible_hits_remaining <= 0

		|| (!possible_hits_remaining && !is_light_surface && !is_solid_surface && enter_material != (int)CHAR_TEX_GRATE && enter_material != (int)CHAR_TEX_GLASS)
		|| weapon_data->flPenetration <= 0.f
		|| !trace_to_exit(enter_trace, exit_trace, enter_trace.endpos, direction)
		&& !(interfaces::trace->GetPointContents(enter_trace.endpos, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL))
		return false;

	surfacedata_t* exit_surface_data = interfaces::physsurface->GetSurfaceData(exit_trace.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;
	float exit_damage_modifier = exit_surface_data->game.flDamageModifier;


	if (sv_penetration_type)
	{
		if (enter_material == (int)CHAR_TEX_GRATE || enter_material == (int)CHAR_TEX_GLASS)
		{
			combined_penetration_modifier = 3.f;
			final_damage_modifier = 0.05f;
		}
		else if (is_solid_surface || is_light_surface)
		{
			combined_penetration_modifier = 1.f;
			final_damage_modifier = 0.16f;
		}
		else if (enter_material == (int)CHAR_TEX_FLESH && (game::main.get_local()->m_iTeamNum() == enemy->m_iTeamNum() && ff_damage_reduction_bullets == 0.f))
		{
			if (ff_damage_bullet_penetration == 0.f)
				return false;

			combined_penetration_modifier = ff_damage_bullet_penetration;
			final_damage_modifier = 0.16f;
		}
		else
		{
			combined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) / 2.f;
			final_damage_modifier = 0.16f;
		}


		if (enter_material == exit_material)
		{
			if (exit_material == (int)CHAR_TEX_CARDBOARD || exit_material == (int)CHAR_TEX_WOOD)
				combined_penetration_modifier = 3.f;
			else if (exit_material == (int)CHAR_TEX_PLASTIC)
				combined_penetration_modifier = 2.f;
		}


		thickness = (exit_trace.endpos - enter_trace.endpos).LengthSqr();
		modifier = fmaxf(1.f / combined_penetration_modifier, 0.f);

		lost_damage = fmaxf(
			((modifier * thickness) / 24.f)
			+ ((current_damage * final_damage_modifier)
				+ (fmaxf(3.75f / penetration_power, 0.f) * 3.f * modifier)), 0.f);


		if (lost_damage > current_damage)
			return false;

		if (lost_damage > 0.f)
			current_damage -= lost_damage;


		if (current_damage < 1.f)
			return false;

		eye_position = exit_trace.endpos;
		--possible_hits_remaining;

		return true;
	}
	else
	{
		combined_penetration_modifier = 1.f;

		if (is_solid_surface || is_light_surface)
			final_damage_modifier = 0.99f;
		else
		{
			final_damage_modifier = fminf(enter_damage_modifier, exit_damage_modifier);
			combined_penetration_modifier = fminf(enter_surf_penetration_modifier, exit_surf_penetration_modifier);
		}

		if (enter_material == exit_material && (exit_material == (int)CHAR_TEX_METAL || exit_material == (int)CHAR_TEX_WOOD))
			combined_penetration_modifier += combined_penetration_modifier;

		thickness = (exit_trace.endpos - enter_trace.endpos).LengthSqr();

		if (sqrt(thickness) <= combined_penetration_modifier * penetration_power)
		{
			current_damage *= final_damage_modifier;
			eye_position = exit_trace.endpos;
			--possible_hits_remaining;

			return true;
		}

		return false;
	}
}
bool c_autowall::classname_is(c_base_entity* entity, const char* class_name) {
	return fnv_hash(entity->get_classname()) == fnv_hash(class_name);
}

bool c_autowall::is_breakable(c_base_entity* entity) {
	static auto __rtdynamiccast_fn = utils::pattern_scan("client.dll", "6A 18 68 ? ? ? ? E8 ? ? ? ? 8B 7D 08");
	static auto is_breakable_entity_fn = utils::pattern_scan("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE");
	static auto multiplayerphysics_rtti_desc = *(uintptr_t*)((uintptr_t)is_breakable_entity_fn + 0x50);
	static auto baseentity_rtti_desc = *(uintptr_t*)((uintptr_t)is_breakable_entity_fn + 0x55);
	static auto breakablewithpropdata_rtti_desc = *(uintptr_t*)((uintptr_t)is_breakable_entity_fn + 0xD5);

	int(__thiscall * **v4)(c_base_entity*);
	int v5;

	if (entity && (*(DWORD*)(entity + 256) >= 0 || (*(int(**)(void))(*(DWORD*)entity + 122))() <= 0) && *(BYTE*)(*(DWORD*)entity + 640) == 2) {
		auto v3 = *(DWORD*)(*(DWORD*)entity + 1136);

		if (v3 != 17 && v3 != 6 && v3)
			return false;

		if (*(DWORD*)(*(DWORD*)entity + 256) > 200)
			return false;

		__asm {
			push 0
			push multiplayerphysics_rtti_desc
			push baseentity_rtti_desc
			push 0
			push entity
			call __rtdynamiccast_fn
			add esp, 20
			mov v4, eax
		}

		if (v4) {
			if ((**v4)(entity) != 1)
				return false;

			goto label_18;
		}

		if (!classname_is(entity, "func_breakable") && !classname_is(entity, "func_breakable_surf")) {
			if ((*((int(__thiscall**)(c_base_entity*)) * (DWORD*)entity + 592))(entity))
				return false;

			goto label_18;
		}

		if (!classname_is(entity, "func_breakable_surf") || !*((uint8_t*)entity + 2564)) {
		label_18:
			__asm {
				push 0
				push breakablewithpropdata_rtti_desc
				push baseentity_rtti_desc
				push 0
				push entity
				call __rtdynamiccast_fn
				add esp, 20
				mov v5, eax
			}

			if (v5 && ((float(__thiscall*)(uintptr_t)) * (uintptr_t*)(*(uintptr_t*)v5 + 12))(v5) <= 0.0f)
				return true;
		}
	}

	return false;
}

void c_autowall::get_bullet_type(float& max_range, float& max_distance, char* bullet_type, bool sv_penetration_type)
{
	if (sv_penetration_type)
	{
		max_range = 35.0;
		max_distance = 3000.0;
	}
	else
	{
		switch (fnv_hash(bullet_type))
		{
		case fnv_hash("BULLET_PLAYER_338MAG"):
			max_range = 45.0;
			max_distance = 8000.0;
			break;
		case fnv_hash("BULLET_PLAYER_762MM"):
			max_range = 39.0;
			max_distance = 5000.0;
			break;
		case fnv_hash("BULLET_PLAYER_556MM"):
			max_range = 35.0;
			max_distance = 4000.0;
			break;
		case fnv_hash("BULLET_PLAYER_556MM_SMALL"):
			max_range = 35.0;
			max_distance = 4000.0;
			break;
		case fnv_hash("BULLET_PLAYER_556MM_BOX"):
			max_range = 35.0;
			max_distance = 4000.0;
			break;
		case fnv_hash("BULLET_PLAYER_57MM"):
			max_range = 30.0;
			max_distance = 2000.0;
			break;
		case fnv_hash("BULLET_PLAYER_50AE"):
			max_range = 30.0;
			max_distance = 1000.0;
			break;
		case fnv_hash("BULLET_PLAYER_357SIG"):
			max_range = 25.0;
			max_distance = 800.0;
			break;
		case fnv_hash("BULLET_PLAYER_357SIG_SMALL"):
			max_range = 25.0;
			max_distance = 800.0;
			break;
		case fnv_hash("BULLET_PLAYER_357SIG_P250"):
			max_range = 25.0;
			max_distance = 800.0;
			break;
		case fnv_hash("BULLET_PLAYER_357SIG_MIN"):
			max_range = 25.0;
			max_distance = 800.0;
			break;
		case fnv_hash("BULLET_PLAYER_9MM"):
			max_range = 21.0;
			max_distance = 800.0;
			break;
		case fnv_hash("BULLET_PLAYER_45ACP"):
			max_range = 15.0;
			max_distance = 500.0;
			break;
		case fnv_hash("BULLET_PLAYER_BUCKSHOT"):
			max_range = 0.0;
			max_distance = 0.0;
			break;
		}
	}
}

bool c_autowall::trace_to_exit_short(Vector& point, Vector& dir, const float step_size, float max_distance)
{
	float flDistance = 0;

	while (flDistance <= max_distance)
	{
		flDistance += step_size;

		point += dir * flDistance;
		int point_contents = interfaces::trace->GetPointContents(point, MASK_SHOT_HULL);
		if (!(point_contents & MASK_SHOT_HULL))
			return true;

	}
	return false;
}

float c_autowall::get_thickness(Vector& start, Vector& end) {
	Vector dir = end - start;
	Vector step = start;
	dir /= dir.Length();

	CTraceFilter filter;
	filter.pSkip = nullptr;

	trace_t trace;

	float thickness = 0;
	while (true) {
		Ray_t ceau;
		ceau.Init(step, end);
		interfaces::trace->TraceRay(ceau, MASK_SOLID, &filter, &trace);

		if (!trace.DidHit())
			break;

		const Vector lastStep = trace.endpos;
		step = trace.endpos;

		if ((end - start).Length() <= (step - start).Length())
			break;

		if (!trace_to_exit_short(step, dir, 5, 90))
			return 0.f;

		thickness += (step - lastStep).Length();
	}
	return thickness;
}

bool c_autowall::trace_to_exit(trace_t& enter_trace, trace_t& exit_trace, Vector start_position, Vector direction)
{
	Vector start, end;
	float max_distance = 90.f, ray_extension = 4.f, current_distance = 0;
	int first_contents = 0;

	while (current_distance <= max_distance)
	{
		current_distance += ray_extension;
		start = start_position + direction * current_distance;

		if (!first_contents)
			first_contents = interfaces::trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		int point_contents = interfaces::trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(point_contents & MASK_SHOT_HULL) || point_contents & CONTENTS_HITBOX && point_contents != first_contents)
		{

			end = start - (direction * ray_extension);

			trace_line(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exit_trace);


			if (exit_trace.startsolid && exit_trace.surface.flags & SURF_HITBOX)
			{
				trace_line(start, start_position, MASK_SHOT_HULL, (c_base_entity*)exit_trace.hit_entity, &exit_trace);

				if (exit_trace.DidHit() && !exit_trace.startsolid)
				{
					start = exit_trace.endpos;
					return true;
				}

				continue;
			}

			if (exit_trace.DidHit() && !exit_trace.startsolid)
			{
				if (is_breakable((c_base_entity*)enter_trace.hit_entity) && is_breakable((c_base_entity*)exit_trace.hit_entity))
					return true;

				if (enter_trace.surface.flags & SURF_NODRAW || !(exit_trace.surface.flags & SURF_NODRAW) && (exit_trace.plane.normal.Dot(direction) <= 1.f))
				{
					float mult_amount = exit_trace.fraction * 4.f;
					start -= direction * mult_amount;
					return true;
				}

				continue;
			}

			if (!exit_trace.DidHit() || exit_trace.startsolid)
			{
				if (enter_trace.DidHit() && is_breakable((c_base_entity*)enter_trace.hit_entity))
				{
					exit_trace = enter_trace;
					exit_trace.endpos = start + direction;
					return true;
				}
				continue;
			}
		}
	}
	return false;
}



bool c_autowall::is_armored(c_base_entity* player, int armor, int hitgroup) {
	if (!player)
		return false;

	bool result = false;

	if (armor > 0) {
		switch (hitgroup) {
		case (int)HITGROUP_GENERIC:
		case (int)HITGROUP_CHEST:
		case (int)HITGROUP_STOMACH:
		case (int)HITGROUP_LEFTARM:
		case (int)HITGROUP_RIGHTARM:
			result = true;
			break;
		case (int)HITGROUP_HEAD:
			result = player->m_bHasHelmet();
			break;
		}
	}

	return result;
}

float c_autowall::get_damage_vec(Vector point) {

	game::main.in_trace = true;

	fire_bullet_data_t data;
	data.filter = CTraceFilter();
	data.filter.pSkip = game::main.get_local();
	data.src = c_prediction::get().unpredicted_eye_pos;
	Vector angles, direction;
	Vector tmp = point - data.src;
	float currentDamage = 0;
	angles = math::vector_angles(tmp);
	direction = math::i_angle_t_vector_i(angles);

	direction.NormalizeInPlace();

	if (fire_bullet(game::main.get_local()->m_hActiveWeapon().Get(), direction, currentDamage, data))
	{
		game::main.in_trace = false;
		return currentDamage;
	}

	game::main.in_trace = false;
	return 0.f;
}


void c_autowall::trace_line(Vector& vec_abs_start, Vector& vec_abs_end, unsigned int mask, c_base_entity* ignore, trace_t* ptr)
{
	CTraceFilter filter;
	filter.pSkip = ignore;
	Ray_t samp;
	samp.Init(vec_abs_start, vec_abs_end);
	interfaces::trace->TraceRay(samp, mask, &filter, ptr);
}

