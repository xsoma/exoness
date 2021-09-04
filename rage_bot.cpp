#include "rage_bot.h"
#include "auto_wall.h"
#include "menu.h"
#include "mathematics.h"
#include "prediction.h"
#include "lag_comp.h"
#include "miss_count.h"
#include "frame_stage_main.h"

void c_rage::on_create_move(CUserCmd* m_pcmd)
{
	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected())
		return;

	if (!game::main.get_local() || !game::main.get_local()->is_alive() || !game::main.get_local()->m_hActiveWeapon().Get())
		return;

	interfaces::engine->GetViewAngles(engine_angles);
	this->update();

	if (game::main.get_local()->m_hActiveWeapon().Get()->is_non_aim() || game::main.get_local()->m_hActiveWeapon().Get()->m_iItemDefinitionIndex() == WEAPON_TASER)
		return;

	this->scan();
	this->aim();
}

void c_rage::exploits_run()
{
	if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected())
		return;

	if (!game::main.get_local() || !game::main.get_local()->is_alive() || !game::main.get_local()->m_hActiveWeapon())
		return;

	if (c_config::get().active_exploit == 1)
	{
		auto get = utils::method_key_bind(c_config::get().hold_dt, c_config::get().dt_key);

		if (get)
		{
			auto can_shift_tick = []() -> bool
			{
				float m_flPlayerTime = (game::main.get_local()->m_nTickBase() - ((game::main.m_shift > 0) ? 1 + game::main.m_shift : 0))* interfaces::global_vars->interval_per_tick;

				if (m_flPlayerTime < game::main.get_local()->m_hActiveWeapon().Get()->m_flNextAttack() || m_flPlayerTime < game::main.get_local()->m_hActiveWeapon().Get()->m_flNextPrimaryAttack())
					return false;

				return true;
			};

			static int backup_tickcount = 0;
			int dt_tick = interfaces::global_vars->tickcount - backup_tickcount;

			if (can_shift_tick())
			{
				if (dt_tick >= time_to_ticks(0.2f) && game::main.get_command()->buttons & IN_ATTACK) {
					game::main.m_shift = 14;
					backup_tickcount = interfaces::global_vars->tickcount;
				}
			}
			
		}
	}
}

void c_rage::scan()
{
	target_data.reset();

	for (int i = 1; i <= interfaces::global_vars->maxClients; i++) {

		c_base_entity* pPlayerEntity = static_cast<c_base_entity*>( interfaces::entitylist->GetClientEntity(i));

		if (!pPlayerEntity
			|| !pPlayerEntity->is_alive()
			|| pPlayerEntity->IsDormant())
		{
			lag_comp::get().clear(i);
			continue;
		}

		if (pPlayerEntity == game::main.get_local() || pPlayerEntity->m_iTeamNum() == game::main.get_local()->m_iTeamNum())
			continue;

		if (pPlayerEntity->m_bGunGameImmunity() || pPlayerEntity->m_fFlags() & FL_FROZEN)
		{
			lag_comp::get().clear(i);
			continue;
		}

		auto intervals = interfaces::global_vars->interval_per_tick * 2.0f;
		auto unlag = fabs(pPlayerEntity->m_flSimulationTime() - pPlayerEntity->m_flOldSimulationTime()) < intervals;

		if (!unlag)
			continue;
		
		lag_comp::get().store_record(pPlayerEntity);

		if (lag_comp::get().PlayerRecord[i].size() == 0)
			continue;

		Vector location = best_point(pPlayerEntity);

		if (location == Vector(0, 0, 0))
			continue;

		float fov = math::get_fov(engine_angles, math::calculate_angle(c_prediction::get().unpredicted_eye_pos, location));

		if (fov > c_config::get().max_fov)
			continue;

		float sim_time = 0.f;
		
		get_target(pPlayerEntity, fov, location, sim_time);
	}
}

void c_rage::get_target(c_base_entity* e, float fov, Vector location, float simulation_time)
{
	switch (c_config::get().target_selection) {
	case 1:
		if (target_data.best_distance < fov)
			return;

		target_data.best_distance = fov;
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;

		break;
	case 2:
		if (target_data.best_distance < e->m_iHealth())
			return;

		target_data.best_distance = e->m_iHealth();
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;

		break;
	case 3:
		float distance = sqrt(
			pow(double(location.x - game::main.get_local()->m_vecOrigin().x), 2) +
			pow(double(location.y - game::main.get_local()->m_vecOrigin().y), 2) +
			pow(double(location.z - game::main.get_local()->m_vecOrigin().z), 2));

		if (target_data.best_distance < distance)
			return;

		target_data.best_distance = distance;
		target_data.tid = e->EntIndex();
		target_data.aimspot = location;
		target_data.simtime = simulation_time;

		break;
	}
}

inline float asdasd(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}

#define square( x ) ( x * x )

void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(asdasd(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

bool c_rage::head_valid(c_base_entity* e)
{
	if (c_config::get().baim_air && !(e->m_fFlags() & FL_ONGROUND))
		return false;

	if (c_config::get().force)
	{
		if (e->m_flDuckAmount() > 0)
			return false;

		if (frame::get().infos[e->EntIndex()].abs_miss >= 2)
			return false;
	}

	if (c_config::get().baim_lethal)
	{
		if (c_autowall::get().get_damage_vec(e->hitbox_position(HITBOX_PELVIS)) >= e->m_iHealth())
			return false;
		else if (c_autowall::get().get_damage_vec(e->hitbox_position(HITBOX_STOMACH)) >= e->m_iHealth())
			return false;
		else if (c_autowall::get().get_damage_vec(e->hitbox_position(HITBOX_CHEST)) >= e->m_iHealth())
			return false;
		else if (c_autowall::get().get_damage_vec(e->hitbox_position(HITBOX_LOWER_CHEST)) >= e->m_iHealth())
			return false;
	}

	return true;
}

void AngleVectors(const Vector& angles, Vector* forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void VectorAngles(const Vector& vecForward, Vector& vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / 3.14159265358979323846f;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / 3.14159265358979323846f;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void c_rage::aim()
{
	if (target_data.tid == -1)
		return;

	auto e = static_cast<c_base_entity*>(interfaces::entitylist->GetClientEntity(target_data.tid));

	float flServerTime = game::main.get_local()->m_nTickBase() * interfaces::global_vars->interval_per_tick;
	bool canShoot = (game::main.get_local()->m_hActiveWeapon().Get()->m_flNextPrimaryAttack() <= flServerTime) && game::main.get_local()->m_hActiveWeapon().Get()->m_iClip1() > 0;

	static int MinimumVelocity = game::main.get_local()->m_hActiveWeapon().Get()->GetCSWeaponData()->flSpreadAlt * .34f;

	if (spread_limit() && canShoot)
	{
		Vector next_angle = math::calculate_angle(c_prediction::get().unpredicted_eye_pos, target_data.aimspot);

		bool hitchanced = hitchance(next_angle, e, config.min_hit);

		float SimulationTime = 0.f;

		if (lag_comp::get().PlayerRecord[target_data.tid].size() != 0)
			SimulationTime = lag_comp::get().PlayerRecord[target_data.tid].at(lag_comp::get().PlayerRecord[target_data.tid].size() - 1).SimTime;

		if (hitchanced) {

			game::main.send_packet = true;

			game::main.get_command()->viewangles = next_angle - (game::main.get_local()->m_aimPunchAngle() * interfaces::cvar->FindVar("weapon_recoil_scale")->GetFloat());

			game::main.get_command()->buttons |= IN_ATTACK;

			game::main.get_command()->tick_count = time_to_ticks(SimulationTime + lag_comp::get().lerp_time());

			miss_count::get().set_data(e, target_data.tid, next_angle);
		}
	}

	if (game::main.get_local()->m_vecVelocity().Length() >= MinimumVelocity && game::main.get_local()->m_fFlags() & FL_ONGROUND)
	{
		Vector Velocity = game::main.get_local()->m_vecVelocity();

		if (Velocity.Length2D() == 0)
			return;

		static float Speed = 450.f;

		Vector Direction;
		Vector RealView;
		VectorAngles(Velocity, Direction);
		interfaces::engine->GetViewAngles(RealView);
		Direction.y = RealView.y - Direction.y;

		Vector Forward;
		AngleVectors(Direction, &Forward);
		Vector NegativeDirection = Forward * -Speed;

		game::main.get_command()->forwardmove = NegativeDirection.x;
		game::main.get_command()->sidemove = NegativeDirection.y;
	}

}

mstudiobbox_t* zhitbox(c_base_entity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->m_iHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = interfaces::modelinfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}

void c_rage::generate_points(c_base_entity* entity, int hitbox_id, std::vector< Vector >& points) {
	auto hitbox = zhitbox(entity, hitbox_id);
	if (!hitbox) return;
	auto bones = entity->get_bone_matrix(hitbox->bone);
	Vector min, max;
	math::vector_transform(hitbox->bbmin, bones, min);
	math::vector_transform(hitbox->bbmax, bones, max);
	Vector
		center = (min + max) * 0.5f,
		angle = math::calculate_angle(center, c_prediction::get().unpredicted_eye_pos);
	Vector forward;
	math::angle_vectors(angle, forward);
	Vector
		right = forward.CrossProduct(Vector(0, 0, 1)),
		left = Vector(-right.x, -right.y, right.z),
		top = Vector(0, 0, 1),
		bot = Vector(0, 0, -1);

	float normalize_scale_head = config.head_scale / 100.f;
	float normalize_scale_body = config.body_scale / 100.f;

	switch (hitbox_id)
	{

	case HITBOX_HEAD:
	{
		points.push_back(center);

		if (!utils::is_visible(center, false) || normalize_scale_head < 0.1f) {
			for (int i = 0; i < 4; i++)
				points.push_back(center);

			points[1] += top * (hitbox->m_flRadius * normalize_scale_head);
			points[2] += right * (hitbox->m_flRadius * normalize_scale_head);
			points[3] += left * (hitbox->m_flRadius * normalize_scale_head);

		}
		break;
	}

	case HITBOX_UPPER_CHEST: {
		points.push_back(center);
		if (!utils::is_visible(center, false) || normalize_scale_body < 0.1f) {
			for (int i = 0; i < 3; i++)
				points.push_back(center);

			points[1] += right * (hitbox->m_flRadius * normalize_scale_body);
			points[2] += left * (hitbox->m_flRadius * normalize_scale_body);
		}

		break;
	}
	case HITBOX_LOWER_CHEST:
	{

		points.push_back(center);
		if (!utils::is_visible(center, false) || normalize_scale_body < 0.1f) {
			for (int i = 0; i < 3; i++)
				points.push_back(center);

			points[1] += right * (hitbox->m_flRadius * normalize_scale_body);
			points[2] += left * (hitbox->m_flRadius * normalize_scale_body);
		}

		break;
	}

	case HITBOX_PELVIS: {

		points.push_back(center);
		if (!utils::is_visible(center, false) || normalize_scale_body < 0.1f) {
			for (int i = 0; i < 3; i++)
				points.push_back(center);

			points[1] += right * (hitbox->m_flRadius * normalize_scale_body);
			points[2] += left * (hitbox->m_flRadius * normalize_scale_body);
		}

		break;
	}

	case HITBOX_RIGHT_FOOT:
	case HITBOX_LEFT_FOOT:
	case HITBOX_NECK:
	{
		points.push_back(center);
		break;
	}
	
	default: {
		points.push_back(center);
		break;
	}

	}
}

Vector c_rage::best_point(c_base_entity* pEnt)
{
	std::vector<int> Scan;

	if (config.prior_hitbox == 1)
	{
		if (head_valid(pEnt))
			Scan.push_back(HITBOX_HEAD);
	}
	else if (config.prior_hitbox == 2)
		Scan.push_back(HITBOX_PELVIS);

	if (head_valid(pEnt))
	{
		Scan.push_back(HITBOX_HEAD);
		Scan.push_back(HITBOX_NECK);
	}

	Scan.push_back(HITBOX_UPPER_CHEST);
	Scan.push_back(HITBOX_LOWER_CHEST);

	Scan.push_back(HITBOX_PELVIS);
	Scan.push_back(HITBOX_CHEST);

	Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
	Scan.push_back(HITBOX_LEFT_UPPER_ARM);

	Scan.push_back(HITBOX_RIGHT_FOREARM);
	Scan.push_back(HITBOX_LEFT_FOREARM);

	Scan.push_back(HITBOX_RIGHT_HAND);
	Scan.push_back(HITBOX_LEFT_HAND);

	Scan.push_back(HITBOX_RIGHT_THIGH);
	Scan.push_back(HITBOX_LEFT_THIGH);

	Scan.push_back(HITBOX_RIGHT_CALF);
	Scan.push_back(HITBOX_LEFT_CALF);

	Scan.push_back(HITBOX_RIGHT_FOOT);
	Scan.push_back(HITBOX_LEFT_FOOT);

	Vector vector_best_point = Vector(0, 0, 0);

	auto minimum_damage = math::clamp(config.min_damage, 1, pEnt->m_iHealth());
	auto best_damage = 0;

	for (auto current : Scan)
	{
		std::vector <Vector> points;
		generate_points(pEnt, current, points);

		for (auto point : points)
		{
			if (utils::is_any_visible(c_prediction::get().unpredicted_eye_pos, point, pEnt, game::main.get_local()))
			{
				auto damage = c_autowall::get().get_damage_vec(point);

				if (damage > best_damage&& damage >= minimum_damage)
				{
					best_damage = damage;
					vector_best_point = point;
					if (damage > pEnt->m_iHealth() + 4)
						return vector_best_point;
				}
			}
			else
			{
				auto damage = c_autowall::get().get_damage_vec(point);

				if (damage > best_damage&& damage >= minimum_damage)
				{
					best_damage = damage;
					vector_best_point = point;
					if (damage > (pEnt->m_iHealth() + 15))
						return vector_best_point;
				}
			}
	
		}

		auto weapon = pEnt->m_hActiveWeapon().Get();

		if (current == HITBOX_HEAD && vector_best_point != Vector(0.0f, 0.0f, 0.0f) && weapon)
		{
			auto last_shot_time = time_to_ticks(pEnt->m_flSimulationTime() - weapon->m_fLastShotTime());

			if (last_shot_time <= 1)
				return vector_best_point;
		}
	}

	return vector_best_point;
}

bool c_rage::spread_limit()
{
	if (!(game::main.get_local()->m_fFlags() & FL_ONGROUND))
		return true;

	auto inaccuracy = game::main.get_local()->m_hActiveWeapon().Get()->inaccuracy();

	if (!inaccuracy)
		inaccuracy = 0.0000001f;

	inaccuracy = 1.0f / inaccuracy;

	auto chance = config.min_hit * 1.5f;

	if (!chance)
		chance = 150.0f;

	return inaccuracy > chance;
	
	return true;
}

void c_rage::update()
{
	auto id = game::main.get_local()->m_hActiveWeapon().Get()->m_iItemDefinitionIndex();

	int element = -1;

	if (id == ItemDefinitionIndex::WEAPON_G3SG1 || id == ItemDefinitionIndex::WEAPON_SCAR20)
		element = 0;

	else if (id == ItemDefinitionIndex::WEAPON_SSG08)
		element = 1;

	else if (id == ItemDefinitionIndex::WEAPON_AWP)
		element = 2;

	else if (game::main.get_local()->m_hActiveWeapon().Get()->is_pistol() && id != ItemDefinitionIndex::WEAPON_REVOLVER && id != ItemDefinitionIndex::WEAPON_DEAGLE)
		element = 3;

	else if (id == ItemDefinitionIndex::WEAPON_REVOLVER || id == ItemDefinitionIndex::WEAPON_DEAGLE)
		element = 4;

	else
		element = 5;

	config.head_scale = c_config::get().head_scale[element];
	config.body_scale = c_config::get().body_scale[element];
	config.prior_hitbox = c_config::get().priority[element];
	config.min_damage = c_config::get().damage[element];
	config.min_hit = c_config::get().hitchance[element];

}

bool c_rage::hitchance(Vector angle, c_base_entity* e, float chance) {

	if (!game::main.get_local()) 
		return false;

	int traces_hit = 0;
	int needed_hits = static_cast<int>(150.f * (chance / 100.f));
	Vector forward, right, up;
	const Vector eye_position = c_prediction::get().unpredicted_eye_pos;
	math::angle_vectors(angle, &forward, &right, &up);

	c_base_combat_weapon* weapon = game::main.get_local()->m_hActiveWeapon().Get();

	if (!weapon)
		return false;

	weapon->penalty();
	float weapon_spread = weapon->spread();
	float weapon_cone = weapon->inaccuracy();

	const auto get_bullet_location = [&](int seed) {

		math::random_seed(seed);

		float a = math::random_float(0.f, 1.f);
		float b = math::random_float(0.f, 2.f * M_PI);
		float c = math::random_float(0.f, 1.f);
		float d = math::random_float(0.f, 2.f * M_PI);

		const float generated_spread = a * weapon_spread;
		const float generated_cone = c * weapon_cone;

		const Vector spread = Vector(
			std::cos(b) * generated_spread + std::cos(d) * generated_cone,
			std::sin(b) * generated_spread + std::sin(d) * generated_cone,
			0.f
		);

		return Vector(forward + right * -spread.x + up * -spread.y).Normalized();
	};

	for (int i = 1; i <= 1500; i++) {
		Vector spread_angle;
		Vector bullet_end;

		spread_angle = math::vector_angles(get_bullet_location(i));
		bullet_end = math::i_angle_t_vector_i(angle - (spread_angle - angle));

		trace_t trace;
		Ray_t ray;
		ray.Init(eye_position, eye_position + bullet_end * weapon->GetCSWeaponData()->flRange);

		interfaces::trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, e, &trace);

		if (trace.hit_entity == e)
			++traces_hit;

		if (static_cast<int>((static_cast<float>(traces_hit) / 150.f) * 100.f) >= chance)
			return true;

		if ((150 - i + traces_hit) < needed_hits)
			return false;
	}

	return false;
}

Vector c_rage::angle_reverse(c_base_entity* pEntity, Vector& vPunchAngleVelocity)
{
	Vector vPunchAngle, vPunchAngleExpDecay;
	float flExpDecay, flExpDecayResult;
	float flLinDecay, flLinDecayResult;

	float tick_interval_;
	float flPunchAngleExpDecayLength;

	ConVar* weapon_recoil_decay2_exp;
	ConVar* weapon_recoil_decay2_lin;
	ConVar* weapon_recoil_vel_decay;

	ConVar* weapon_recoil_scale;

	weapon_recoil_decay2_exp = interfaces::cvar->FindVar("weapon_recoil_decay2_exp");
	weapon_recoil_decay2_lin = interfaces::cvar->FindVar("weapon_recoil_decay2_lin");
	weapon_recoil_vel_decay = interfaces::cvar->FindVar("weapon_recoil_vel_decay");

	weapon_recoil_scale = interfaces::cvar->FindVar("weapon_recoil_scale");

	Vector& rPunchAngle = pEntity->m_aimPunchAngle();
	vPunchAngle[0] = rPunchAngle[0];
	vPunchAngle[1] = rPunchAngle[1];
	vPunchAngle[2] = rPunchAngle[2];

	flExpDecay = weapon_recoil_decay2_exp->GetFloat();
	flLinDecay = weapon_recoil_decay2_lin->GetFloat();

	tick_interval_ = interfaces::global_vars->interval_per_tick;

	flExpDecayResult = exp(-flExpDecay * tick_interval_);

	vPunchAngleExpDecay = vPunchAngle * flExpDecayResult;
	flPunchAngleExpDecayLength = vPunchAngleExpDecay.Length();

	flLinDecayResult = flLinDecay * tick_interval_;

	if ((flPunchAngleExpDecayLength > 0.0) && (flPunchAngleExpDecayLength > flLinDecayResult))
	{
		float flMultiplier;

		flMultiplier = 1.0f - (flLinDecayResult / flPunchAngleExpDecayLength);
		vPunchAngleExpDecay *= flMultiplier;
	}
	else
	{
		vPunchAngleExpDecay.x = vPunchAngleExpDecay.y = vPunchAngleExpDecay.z = 0;
	}

	vPunchAngle = (vPunchAngleVelocity * tick_interval_ * 0.5) + vPunchAngleExpDecay;

	flExpDecayResult = exp(tick_interval_ * -weapon_recoil_vel_decay->GetFloat());

	vPunchAngleVelocity *= flExpDecayResult;
	vPunchAngle += (vPunchAngleVelocity * tick_interval_ * 0.5f);

	Vector result;
	result[0] = (float)vPunchAngle[0];
	result[1] = (float)vPunchAngle[1];
	result[2] = (float)vPunchAngle[2];

	return result;
}