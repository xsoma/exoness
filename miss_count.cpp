#include "miss_count.h"
#include "prediction.h"
#include "mathematics.h"
#include "logs.h"
#include "frame_stage_main.h"

void miss_count::setup_event(IGameEvent* event)
{
	if (!strcmp(event->GetName(), ("bullet_impact")))
	{
		const auto pos = Vector(event->GetFloat(("x")), event->GetFloat(("y")), event->GetFloat(("z")));
		const auto userid = interfaces::engine->GetPlayerForUserID(event->GetInt(("userid")));

		if (userid == interfaces::engine->GetLocalPlayer() && game::main.get_local())
		{
			static auto last_shot_from = c_prediction::get().unpredicted_eye_pos;
			static auto last_curtime = interfaces::global_vars->curtime;

			if (last_curtime != interfaces::global_vars->curtime)
			{
				last_shot_from = c_prediction::get().unpredicted_eye_pos;
				last_curtime = interfaces::global_vars->curtime;
			}
		}
		else
		{
			c_base_entity* pla = dynamic_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(userid));

			if (pla && pla->is_alive() && !pla->IsDormant())
			{
				if (pla->m_iTeamNum() != game::main.get_local()->m_iTeamNum())
				{
					static auto last_shot_from = pla->get_eye_pos();
					static auto last_curtime = interfaces::global_vars->curtime;

					if (last_curtime != interfaces::global_vars->curtime)
					{
						last_shot_from = pla->get_eye_pos();
						last_curtime = interfaces::global_vars->curtime;
					}
				}
				else
				{
					static auto last_shot_from = pla->get_eye_pos();
					static auto last_curtime = interfaces::global_vars->curtime;

					if (last_curtime != interfaces::global_vars->curtime)
					{
						last_shot_from = pla->get_eye_pos();
						last_curtime = interfaces::global_vars->curtime;
					}
				}
			}
		}

		const auto size = last_events.size();

		if (size == 0)
		{
			event_data_t d;
			d.hurt_player = interfaces::global_vars->curtime - last_rbot_shot_time < 2.f ? last_rbot_entity : -1;
			d.pos = pos;
			d.userid = userid;
			d.hurt = false;
			d.time = interfaces::global_vars->curtime;
			d.got_pos = true;
			last_events.push_back(d);
		}
		else
		{
			if (last_events[size - 1].time == interfaces::global_vars->curtime && last_events[size - 1].hurt_player == last_rbot_entity)
				last_events[size - 1].pos = pos;
			else
			{
				event_data_t d;
				d.hurt_player = interfaces::global_vars->curtime - last_rbot_shot_time < 2.f ? last_rbot_entity : -1;
				d.pos = pos;
				d.userid = userid;
				d.hurt = false;
				d.time = interfaces::global_vars->curtime;
				last_events.push_back(d);
			}
		}
	}

	static auto last_hurt_curtime = 0.f;
	static auto last_hurt_attacker = -1;
	static auto last_hurt_userid = -1;
	static auto last_hurt_damage = -1;
	static auto last_hurt_health = -1;
	static auto last_hurt_hitgroup = 0;

	if (!strcmp(event->GetName(), ("player_hurt")))
	{
		const auto attacker = interfaces::engine->GetPlayerForUserID(event->GetInt(("attacker")));
		const auto hurt = interfaces::engine->GetPlayerForUserID(event->GetInt(("userid")));
		const auto health = event->GetInt(("health"));
		const auto dmg_health = event->GetInt(("dmg_health"));
		const auto hitgroup = event->GetInt(("hitgroup"));

		if (last_hurt_curtime == interfaces::global_vars->curtime && last_hurt_attacker == attacker
			&& last_hurt_userid == hurt && last_hurt_damage == dmg_health
			&& last_hurt_health == health && last_hurt_hitgroup == hitgroup)
			return;

		last_hurt_curtime = interfaces::global_vars->curtime;
		last_hurt_attacker = attacker;
		last_hurt_userid = hurt;
		last_hurt_damage = dmg_health;
		last_hurt_health = health;
		last_hurt_hitgroup = hitgroup;

		const auto weapon = event->GetString(("weapon"));

		if (weapon == ("inferno") || weapon == ("hegrenade") || weapon == ("decoy"))
			return;

		for (auto& last_event : last_events)
		{
			if (last_event.userid == attacker && (last_event.hurt_player == hurt || last_event.hurt_player == -1))
			{
				last_event.hurt = true;
				last_event.damage = dmg_health;
				last_event.died = health <= 0;
				last_event.got_hurt = true;
				last_event.hitbox = hitgroup;
			}
		}
	}

}

bool miss_count::valid_to_setup_record(c_base_entity* x)
{
	if (!x->SetupBones(matrixed, 128, BONE_USED_BY_ANYTHING, x->m_flSimulationTime()))
		return false;

	return true;
}

void miss_count::set_data(c_base_entity* player, int index, Vector angle)
{
	if (!player)
		return;

	last_rbot_entity = index;
	last_rbot_shot_time = interfaces::global_vars->curtime;
	last_rbot_shot_eyepos = c_prediction::get().unpredicted_eye_pos;
	last_rbot_shot_angle = angle;
	
}

void miss_count::build_()
{
	if (!game::main.get_local())
	{
		last_events = std::deque< event_data_t >();
		return;
	}

	for (auto& last_event : last_events)
	{
		if (!last_event.got_pos)
			continue;

		auto player = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(last_event.userid));

		if (!player)
			continue;

		if (last_event.hurt_player == -1)
			last_event.hurt_player = last_rbot_entity;

		auto player_hurt = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(last_event.hurt_player));

		if (player == game::main.get_local())
		{
			if (player_hurt && player_hurt->is_alive() && last_event.hurt_player != -1)
			{	
				auto trace_hit_player = false;

				trace_t trace;
				Ray_t ray;
				Vector view;
				const auto angles = math::calculate_angle(last_rbot_shot_eyepos, last_event.pos);
				math::angle_vectors(angles, view);
				view.NormalizeInPlace();

				view = last_rbot_shot_eyepos + view * 4096.f;

				ray.Init(last_rbot_shot_eyepos, view);
				CTraceFilterPlayersOnlySkipOne filter(game::main.get_local());
				filter.ShouldHitEntity(player_hurt, 0);

				interfaces::trace->TraceRay(ray, MASK_ALL | CONTENTS_GRATE, &filter, &trace);

				if (trace.hit_entity)
					trace_hit_player = true;

				if (!last_event.hurt && interfaces::global_vars->curtime - last_rbot_shot_time < 2.f)
				{
					const auto fov = math::get_fov(last_rbot_shot_angle, math::calculate_angle(last_rbot_shot_eyepos, last_event.pos));

					if (!trace_hit_player && fov >= 0.01f && fov < 30.f)
					{
						logs_put::get().add("Missed shot due to spread", Color(255, 255, 255));
					}
					else
					{
						frame::get().infos[player_hurt->EntIndex()].miss++;
						frame::get().infos[player_hurt->EntIndex()].abs_miss++;

						std::stringstream text;

						text << "Missed shot due to desync ";

						std::stringstream info;

						if (frame::get().infos[player_hurt->EntIndex()].r_mode == resolve_process::RESOLVE_SIDE_1)
							info << "( Side: [0] | ";
						else if (frame::get().infos[player_hurt->EntIndex()].r_mode == resolve_process::RESOLVE_SIDE_OPPOSITE)
							info << "( Side: [1] | ";
						else if (frame::get().infos[player_hurt->EntIndex()].r_mode == resolve_process::RESOLVE_SIDE_2)
							info << "( Side: [2] | ";

						std::stringstream aspect;

						if (frame::get().infos[player_hurt->EntIndex()].aspects == resolve_aspects::ASPECT_BREAKER)
							aspect << "Diff: [1] | ";
						else if (frame::get().infos[player_hurt->EntIndex()].aspects == resolve_aspects::ASPECT_NONE)
							aspect << "Diff: [0] | ";

						std::stringstream is_attack;

						if (frame::get().infos[player_hurt->EntIndex()].fire)
							aspect << "Shot: [1] )";
						else
							aspect << "Shot: [0] )";

						logs_put::get().add(text.str() + info.str() + aspect.str() + is_attack.str(), Color(255, 255, 255));
					}
				}
			}
		}
	}

	last_events = std::deque< event_data_t >();
}

bool miss_count::visible_bone(c_base_entity* Target, int Bone)
{
	return false;
}

bool miss_count::player_visible(c_base_entity* target)
{
	for (int i = 0; i < 128; i++)
	{
		if (visible_bone(target, i))
		{
			return true;
		}
	}
	return false;
}