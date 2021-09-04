#include "frame_stage_main.h"
#include "mathematics.h"
#include "animations.h"
#include "lag_comp.h"
#include "menu.h"

void frame::on_frame_stage_notify(ClientFrameStage_t stage)
{
	if (!game::main.get_local() || !interfaces::engine->IsInGame())
		return;

	for (int i = 1; i < interfaces::engine->GetMaxClients(); ++i)
	{
		c_base_entity* e = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(i));

		if (!e
			|| !e->is_alive()
			|| e == game::main.get_local()
			|| e->IsDormant()
			)
			continue;

		if (!game::main.get_local())
			continue;

		switch (stage) {
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			
			e->set_abs_origin(e->m_vecOrigin());
			
			// e->update_client_side_animation();

			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:

			

			break;
		case FRAME_NET_UPDATE_END:
			
			if (game::main.get_local()->is_alive())
			{
				if (e->m_iTeamNum() != game::main.get_local()->m_iTeamNum())
				{
					if (c_config::get().aimbot_enabled)
					{
						this->update_info(e);

						this->main_part(e);

						// e->update_client_side_animation();
						//this->resolve(e);
					}
				}
			}

			if (e)
			{
				auto var_map = reinterpret_cast<uintptr_t>(e) + 36;
				auto var_map_size = *reinterpret_cast<int*>(var_map + 20);

				for (auto index = 0; index < var_map_size; index++)
					*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = 0;
			}

			break;
		case FRAME_RENDER_START:
			
			

			break;
		}

	}
}

float frame::server_feet_yaw(c_base_entity* entity)
{
	auto animstate = entity->get_animation_state();

	if (!animstate)
		return 0.f;

	/* data */
	auto ground_fraction = *(float*)(animstate + 0x11C);
	auto duck_ammount = *(float*)(animstate + 0xA4);
	auto ducking_speed = std::max(0.f, std::min(1.f, *reinterpret_cast<float*> (animstate + 0xFC)));
	auto running_speed = std::max(0.f, std::min(*reinterpret_cast<float*> (animstate + 0xF8), 1.f));
	/* offsets */
	auto backup_eflags = entity->m_iEflags();

	entity->m_iEflags() = (1 << 12);
	auto abs_velocity = *reinterpret_cast<Vector*> (uintptr_t(entity) + 0x94);
	entity->m_iEflags() = backup_eflags;

	auto speed = std::fmin(abs_velocity.Length(), 260.0f);

	auto goal_feet_yaw = animstate->m_flGoalFeetYaw;

	auto angle_diff = [](float destAngle, float srcAngle) -> float
	{
		auto delta = 0.f;

		delta = fmodf(destAngle - srcAngle, 360.0f);

		if (destAngle > srcAngle)
		{
			if (delta >= 180)
				delta -= 360;
		}
		else
		{
			if (delta <= -180)
				delta += 360;
		}

		return delta;
	};

	auto eye_feet_delta = angle_diff(animstate->m_flEyeYaw, goal_feet_yaw);

	auto flYawModifier = (((ground_fraction * -0.3f) - 0.2f) * running_speed) + 1.0f;

	if (duck_ammount > 0.0f)
		flYawModifier = flYawModifier + ((duck_ammount * ducking_speed) * (0.5f - flYawModifier));

	auto flMaxYawModifier = flYawModifier * 58.f;
	auto flMinYawModifier = flYawModifier * -58.f;

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
			goal_feet_yaw = fabs(flMinYawModifier) + animstate->m_flEyeYaw;
	}
	else
		goal_feet_yaw = animstate->m_flEyeYaw - fabs(flMaxYawModifier);

	if (goal_feet_yaw > 5000 || goal_feet_yaw < -5000)
		return 0.f;

	math::normalize_yaw(goal_feet_yaw);

	if (speed > 0.1f || fabs(abs_velocity.z) > 100.0f)
	{
		goal_feet_yaw = math::approach_angle(
			animstate->m_flEyeYaw,
			goal_feet_yaw,
			((ground_fraction * 20.0f) + 30.0f)
			* animstate->m_flLastClientSideAnimationUpdateTime);
	}
	else
	{
		goal_feet_yaw = math::approach_angle(
			entity->m_lby(),
			goal_feet_yaw,
			animstate->m_flLastClientSideAnimationUpdateTime * 100.0f);
	}

	if (goal_feet_yaw > 5000 || goal_feet_yaw < -5000)
		return 0.f;

	math::normalize_yaw(goal_feet_yaw);

	return goal_feet_yaw;
}

bool frame::is_animation_triggers(c_base_entity* player)
{
	auto true_diff = [](float yaw) -> bool
	{
		static float eye;

		if (eye != yaw)
		{
			eye = yaw;
			return true;
		}
		
		return false;
	};

	auto animations = [](c_base_entity * plr) -> bool
	{
		for (int i = 0; i < 13; i++)
		{
			const int activity = plr->sequence_activity(plr->get_animlayers()[i].m_nSequence);

			if (activity == 979)
			{
				return true;
			}
		}

		return false;
	};

	if (true_diff(player->m_angEyeAngles().y) && animations(player))
		return true;

	return false;

	/*
	const auto l = player->get_animlayers()[3];

	if (l.m_flCycle == 0.f || l.m_flWeight == 0.f)
		return false;

	const auto activity = player->sequence_activity(l.m_nSequence);

	if (l.m_flCycle != this->previous_layer.m_flCycle || l.m_flWeight == 1.f)
	{
		if ((activity == 979 && l.m_flCycle < 0.01f) || (l.m_flWeight == 0.f && (this->previous_layer.m_flCycle > 0.92f && l.m_flCycle > 0.92f)))
		{
			this->previous_layer = l;
			return true;
		}
	}

	return false;
	*/
}

void frame::update_info(c_base_entity* e)
{
	if (!game::main.get_local()->is_alive())
		return;

	infos[e->EntIndex()].state = e->get_animation_state();

	if (infos[e->EntIndex()].miss > 3)
		infos[e->EntIndex()].miss = 0;

	switch (infos[e->EntIndex()].miss)
	{
	case 0:
		infos[e->EntIndex()].r_mode = resolve_process::RESOLVE_SIDE_1;
		break;
	case 1:
		infos[e->EntIndex()].r_mode = resolve_process::RESOLVE_SIDE_OPPOSITE;
		break;
	case 2:
		infos[e->EntIndex()].r_mode = resolve_process::RESOLVE_SIDE_2;
		break;
	}

	if (this->is_animation_triggers(e))
		infos[e->EntIndex()].aspects = resolve_aspects::ASPECT_BREAKER;
	else
		infos[e->EntIndex()].aspects = resolve_aspects::ASPECT_NONE;

	static float oldSimtimeShot[65];
	static float storedSimtimeShot[65];
	static float ShotTime[65];

	bool shot = true;
	
	if (storedSimtimeShot[e->EntIndex()] != e->m_flSimulationTime())
	{
		if (e->m_hActiveWeapon().Get() && !e->m_hActiveWeapon().Get()->is_non_aim())
		{
			if (ShotTime[e->EntIndex()] != e->m_hActiveWeapon().Get()->m_fLastShotTime())
			{
				shot = true;
				ShotTime[e->EntIndex()] = e->m_hActiveWeapon().Get()->m_fLastShotTime();
			}
			else
			{
				shot = false;
			}
		}
		else
		{
			shot = false;
			ShotTime[e->EntIndex()] = 0.f;
		}

		oldSimtimeShot[e->EntIndex()] = storedSimtimeShot[e->EntIndex()];
		storedSimtimeShot[e->EntIndex()] = e->m_flSimulationTime();
	}

	infos[e->EntIndex()].fire = shot;

}

void frame::resolve(c_base_entity* e)
{
	auto & data = infos[e->EntIndex()];
	
	if (!data.state)
		return;

	float yaw_delta = e->get_animation_state()->m_flEyeYaw;
	bool is_animations = data.aspects == resolve_aspects::ASPECT_BREAKER;

	float m_flMaxBodyYaw = *(float*)(uintptr_t(data.state) + 0x334);
	float m_flMinBodyYaw = *(float*)(uintptr_t(data.state) + 0x330);

	auto max_desync_delta = std::fmaxf(std::fabsf(m_flMinBodyYaw), std::fabsf(m_flMaxBodyYaw));

	float get_ = 0.f;

	if (data.state->speed_2d > 3.f)
		get_ = max_desync_delta;
	else
		get_ = is_animations ? 90.0 : 60.0;

	if (!data.fire)
	{
		if (data.r_mode == resolve_process::RESOLVE_SIDE_1)
			e->get_animation_state()->m_flGoalFeetYaw = math::normalize_yaw(yaw_delta + get_);

		else if (data.r_mode == resolve_process::RESOLVE_SIDE_OPPOSITE)
			e->get_animation_state()->m_flGoalFeetYaw = math::normalize_yaw(data.state->m_flEyeYaw);

		else if (data.r_mode == resolve_process::RESOLVE_SIDE_2)
			e->get_animation_state()->m_flGoalFeetYaw = math::normalize_yaw(yaw_delta - get_);
	}
	else
		e->get_animation_state()->m_flGoalFeetYaw = math::normalize_yaw(e->m_angEyeAngles().y);
}

void frame::main_part(c_base_entity* entity)
{
	auto state = entity->get_animation_state();
	 
	if (!state)
		return;

	this->resolve(entity);
}