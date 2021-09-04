#include "csgo_alt.h"
#include "main_entity.h"
#include "prediction.h"
#include "animations.h"
#include "config.h"

detour_hook::SetupBones_t detour_hook::original_setup_bones;

struct setup_bones_data_t
{
    matrix3x4_t matrix[128];
    bool got = false;
    float simtime = 0.f;
    float yaw = 0.f;
    Vector pos = Vector(0, 0, 0);
    int last_bonemask = 0;
    float spawntime = 0.f;
    CCSGOPlayerAnimState* new_state = nullptr;
    bool init_animstate = false;
    int last_tickcount = -1;
    float simtime_state = 0.f;
    AnimationLayer m_real_layers[13];
    float m_real_poses[20];
    float goal_feet_yaw = 0.f;
};

enum
{
    EFL_DIRTY_ABSTRANSFORM = (1 << 11)
};

bool __fastcall detour_hook::setup_bones(IClientRenderable* ecx, void* edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float curtime)
{
    if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
        return original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, curtime);

    if (!game::main.get_local())
        return original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, curtime);

    if (ecx && static_cast<IClientRenderable*> (ecx)->GetIClientUnknown())
    {
        auto networkable = static_cast<IClientRenderable*> (ecx)->GetIClientUnknown()->GetClientNetworkable();

        if (networkable && networkable->GetClientClass() && networkable->GetClientClass()->m_ClassID == 40)
        {
            auto player = (c_base_entity*)ecx;
            auto renderable = (IClientRenderable*)ecx;

            const auto i = networkable->EntIndex();
            static setup_bones_data_t data[64];

            if (player)
                if (curtime == 0.f)
                    curtime = player->m_flSimulationTime();


            static auto host_timescale = interfaces::cvar->FindVar("host_timescale");
            const auto old_cur_time = interfaces::global_vars->curtime;
            const auto old_real_time = interfaces::global_vars->realtime;
            const auto old_frame_time = interfaces::global_vars->frametime;
            const auto old_abs_frame_time = interfaces::global_vars->absoluteframetime;
            const auto old_interp_amount = interfaces::global_vars->interpolation_amount;
            const auto old_tick_count = interfaces::global_vars->tickcount;

            if (player && networkable->EntIndex() == interfaces::engine->GetLocalPlayer())
            {
                if (game::main.get_local() && game::main.get_local()->is_alive() && c_config::get().chams_fake)
                {
                    
                    if (!bone_to_world_out)
                    {
                        static auto last_tickcount = 0;

                        bone_mask = BONE_USED_BY_ANYTHING;

                        if (last_tickcount == interfaces::global_vars->tickcount)
                            bone_mask &= ~BONE_USED_BY_ATTACHMENT;

                        last_tickcount = interfaces::global_vars->tickcount;

                        if (player->m_vecVelocity().Length2D() < 0.1f && data[i].simtime == player->m_flSimulationTime()) //& ~BONE_USED_BY_ATTACHMENT {
                            bone_mask &= ~BONE_USED_BY_ATTACHMENT;

                        data[i].simtime = player->m_flSimulationTime();

                        auto& last_animation_framecount = *reinterpret_cast<int*> (uintptr_t(player) + 0xA68);
                        last_animation_framecount = 0;

                        *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 2664) = 0;

                        if (renderable)
                            *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x269C) = 0; //prev_bone_mask

                        bone_mask |= 0x200;
                        player->m_fEffects() |= 8;
                        player->m_iEflags() &= ~EFL_DIRTY_ABSTRANSFORM;

                        static AnimationLayer babackup_layers[13];
                        static float babackup_poses[20];
                        const auto rval = original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, utils::get_estimate_server_time(game::main.get_command()));

                        interfaces::global_vars->curtime = old_cur_time;
                        interfaces::global_vars->realtime = old_real_time;
                        interfaces::global_vars->absoluteframetime = old_abs_frame_time;
                        interfaces::global_vars->tickcount = old_tick_count;
                        interfaces::global_vars->interpolation_amount = old_interp_amount;

                        return rval;
                    }
                  

                  interfaces::global_vars->curtime = utils::get_estimate_server_time(game::main.get_command());
                  interfaces::global_vars->realtime = utils::get_estimate_server_time(game::main.get_command());

                    if (interfaces::global_vars->frametime > 0.0033333334f)
                        interfaces::global_vars->frametime = 0.003333333f;

                    interfaces::global_vars->frametime = //interfaces::global_vars->interval_per_tick * host_timescale->get_float ( );
                    interfaces::global_vars->absoluteframetime = interfaces::global_vars->interval_per_tick * host_timescale->GetFloat();
                    interfaces::global_vars->interpolation_amount = 0;
                    interfaces::global_vars->tickcount = time_to_ticks(interfaces::global_vars->curtime);

                    auto& last_animation_framecount = *reinterpret_cast<int*> (uintptr_t(player) + 0xA68);
                    last_animation_framecount = 0;

                    if (renderable)
                        *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x269C) = 0; //prev_bone_mask

                    bone_mask = BONE_USED_BY_ANYTHING;

                    player->m_fEffects() |= 8;

                    *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 2664) = 0;
                    player->m_iEflags() &= ~EFL_DIRTY_ABSTRANSFORM;
                  

                    static auto offs_bone_mask = NetvarSys::get().GetOffset("DT_BaseAnimating", "m_nForceBone") + 0x20;
                    *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (player) + offs_bone_mask) = 0;

                    player->invalidate_bone_cache();

                    const auto return_val = original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, curtime);

                    interfaces::global_vars->curtime = old_cur_time;
                    interfaces::global_vars->realtime = old_real_time;
                    interfaces::global_vars->frametime = old_frame_time;
                    interfaces::global_vars->absoluteframetime = old_abs_frame_time;
                    interfaces::global_vars->tickcount = old_tick_count;
                    interfaces::global_vars->interpolation_amount = old_interp_amount;

                    return return_val;
                }
            }

            
            if (player && networkable->EntIndex() != interfaces::engine->GetLocalPlayer())
            {
              
                if (!bone_to_world_out)
                    return true;

                if (networkable->IsDormant())
                    return original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, curtime);

                bone_mask = BONE_USED_BY_ANYTHING;

                if (!c_animations::get().m_player_states.at(player->EntIndex()).m_should_recalculate_matrix)
                {
                    if (!c_animations::get().m_player_states.at(player->EntIndex()).m_got_matrix)
                        return false;

                    memcpy(bone_to_world_out, c_animations::get().m_player_states.at(player->EntIndex()).m_matrix, sizeof(matrix3x4_t) * max_bones);

                    return true;
                }
                
                data[i].got = false;

                auto& last_animation_framecount = *reinterpret_cast<int*> (uintptr_t(player) + 0xA68);
                last_animation_framecount = 0;
                static auto offs_bone_mask = NetvarSys::get().GetOffset("DT_BaseAnimating", "m_nForceBone") + 0x20;
                *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (player) + offs_bone_mask) = 0;

               if (renderable)
               {

                  *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x2698) = 0; // last bonemask
                  *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x2EA) = 0;
                  *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x269C) = 0; //prev_bone_mask
               }

                bone_mask |= 0x200;

                if (renderable)
                {
                    *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x26A8) = -1; 
                    *reinterpret_cast<int*> (reinterpret_cast<uintptr_t> (renderable) + 0x268C) = -1;
                }

               *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 2664) = 0;
               *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 0x80) = 0;
                *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 0x84) = 0;
                *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (player) + 0x88) = 0;

               player->set_abs_origin(player->m_vecOrigin());
               player->invalidate_bone_cache();

               *reinterpret_cast<int*> (reinterpret_cast<int> (player) + 236) |= 8;
               const auto return_val = original_setup_bones(ecx, data[i].matrix, 128, bone_mask, curtime);
               *reinterpret_cast<int*> (reinterpret_cast<int> (player) + 236) &= ~8;

                if (return_val)
                {
                    data[i].simtime = curtime;
                    data[i].got = true;
                    data[i].pos = player->m_vecOrigin();

                    memcpy(bone_to_world_out, data[i].matrix, sizeof(matrix3x4_t) * max_bones);

                    data[i].yaw = player->m_angEyeAngles().y;
                }
                
                return return_val;
            }
            
        }
    }

    return original_setup_bones(ecx, bone_to_world_out, max_bones, bone_mask, curtime);
}
