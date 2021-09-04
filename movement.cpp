#include "movement.h"
#include "main_entity.h"
#include "mathematics.h"
#include <algorithm>

#define square(x) ((x)*(x))
#define fastsqr(x)	(sqrt)(x)

void m_movement::run_bhop(CUserCmd* cmd)
{
	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (!game::main.get_local()->is_alive())
		return;

	static auto last_jumped = false;
	static auto should_fake = false;

	if (!last_jumped && should_fake)
	{
		should_fake = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (game::main.get_local()->m_fFlags() & FL_ONGROUND)
		{
			last_jumped = true;
			should_fake = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			last_jumped = false;
		}
	}
	else
	{
		last_jumped = false;
		should_fake = false;
	}

}

void m_movement::run_strafe ( CUserCmd* cmd, int type )
{
	if ( !interfaces::engine->IsConnected( ) || !interfaces::engine->IsInGame( ) )
		return;

	if ( !game::main.get_local() || !game::main.get_local()->is_alive( ) )
		return;

	if ( type == 0 )
		return;

	if (!GetAsyncKeyState(VK_SPACE) || game::main.get_local()->m_vecVelocity().Length2D() < 0.5)
		return;

	if (!(game::main.get_local()->m_fFlags() & FL_ONGROUND))
	{
		if (type == 1)
		{
			cmd->forwardmove = (10000.f / game::main.get_local()->m_vecVelocity().Length2D() > 450.f) ? 450.f : 10000.f / game::main.get_local()->m_vecVelocity().Length2D();
			cmd->sidemove = (cmd->mousedx != 0) ? (cmd->mousedx < 0.0f) ? -450.f : 450.f : (cmd->command_number % 2) == 0 ? -450.f : 450.f;
		}
		else
		{
			static float cl_sidespeed = interfaces::cvar->FindVar("cl_sidespeed")->GetFloat();

			if (fabsf(cmd->mousedx > 2)) {
				cmd->sidemove = (cmd->mousedx < 0.f) ? -cl_sidespeed : cl_sidespeed;
				return;
			}

			if (GetAsyncKeyState('S')) {
				cmd->viewangles.y -= 180;
			}
			else if (GetAsyncKeyState('D')) {
				cmd->viewangles.y -= 90;
			}
			else if (GetAsyncKeyState('A')) {
				cmd->viewangles.y += 90;
			}

			if (!game::main.get_local()->m_vecVelocity().Length2D() > 0.5 || game::main.get_local()->m_vecVelocity().Length2D() == NAN || game::main.get_local()->m_vecVelocity().Length2D() == INFINITE)
			{
				cmd->forwardmove = 400;
				return;
			}

			cmd->forwardmove = math::clamp(5850.f / game::main.get_local()->m_vecVelocity().Length2D(), -400, 400);
			if ((cmd->forwardmove < -400 || cmd->forwardmove > 400))
				cmd->forwardmove = 0;

			const auto vel = game::main.get_local()->m_vecVelocity();
			const float y_vel = RAD2DEG(atan2(vel.y, vel.x));
			const float diff_ang = math::normalize_yaw(cmd->viewangles.y - y_vel);

			cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
			cmd->viewangles.y = math::normalize_yaw(cmd->viewangles.y - diff_ang);
		}
	}

}

void m_movement::run_slow_walk(CUserCmd* get_cmd, float get_speed)
{
	if (get_speed <= 0.f)
		return;

	float min_speed = (float)(fastsqr(square(get_cmd->forwardmove) + square(get_cmd->sidemove) + square(get_cmd->upmove)));
	if (min_speed <= 0.f)
		return;

	if (get_cmd->buttons & IN_DUCK)
		get_speed *= 2.94117647f;

	if (min_speed <= get_speed)
		return;

	float kys = get_speed / min_speed;

	get_cmd->forwardmove *= kys;
	get_cmd->sidemove *= kys;
	get_cmd->upmove *= kys;
}