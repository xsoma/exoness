#include "animations.h"
#include "mathematics.h"
#include "config.h"

void c_animations::manage_fake_animations()
{
    if (!game::main.get_local()->is_alive())
        return;

    static auto handle = game::main.get_local()->GetRefEHandle();
    static auto spawn_time = game::main.get_local()->m_flSpawnTime();

    bool allocate = (!m_fake_state),
        change = (!allocate) && (game::main.get_local()->GetRefEHandle() != handle),
        reset = (!allocate && !change) && (game::main.get_local()->m_flSpawnTime() != spawn_time);

    if (change)
        free(m_fake_state);

    if (reset)
    {
        game::main.get_local()->reset_animation_state(m_fake_state);

        spawn_time = game::main.get_local()->m_flSpawnTime();
    }

    if (allocate || change)
    {
        auto* state = reinterpret_cast<CCSGOPlayerAnimState*>(malloc(sizeof(CCSGOPlayerAnimState)));

        if (state != nullptr)
            game::main.get_local()->create_animation_state(state);

        handle = game::main.get_local()->GetRefEHandle();
        spawn_time = game::main.get_local()->m_flSpawnTime();

        m_fake_state = state;
    }

    if (game::main.send_packet)
    {
        AnimationLayer networked_layers[13];
        std::memcpy(networked_layers, game::main.get_local()->get_animlayers(), sizeof(networked_layers));

        auto backup_abs_angles = game::main.get_local()->get_abs_angles();
        auto backup_poses = game::main.get_local()->m_flPoseParameter();

        game::main.get_local()->update_animation_state(m_fake_state, game::main.real_angle);
        game::main.get_local()->SetupBones(m_fake_matrix, 128, 0x7FF00, interfaces::global_vars->curtime);

        const auto org_tmp = game::main.get_local()->GetRenderOrigin();

        if (c_config::get().interpolate)
        {
            for (auto& i : m_fake_matrix)
            {
                i[0][3] -= org_tmp.x;
                i[1][3] -= org_tmp.y;
                i[2][3] -= org_tmp.z;
            }
        }

        std::memcpy(game::main.get_local()->get_animlayers(), &networked_layers, sizeof(networked_layers));

        game::main.get_local()->m_flPoseParameter() = backup_poses;
        game::main.get_local()->get_abs_angles() = backup_abs_angles;
    }

}


bool fresh_tick()
{
    static int old_tick_count;

    if (old_tick_count != interfaces::global_vars->tickcount)
    {
        old_tick_count = interfaces::global_vars->tickcount;
        return true;
    }

    return false;
}

void c_animations::manage_real_animations()
{
    if (!interfaces::engine->IsInGame() || !interfaces::engine->IsConnected())
        return;

    auto local = game::main.get_local();

    const auto state = local->get_animation_state();

    if (!state)
        return;

    static float sim;
    static std::array<float, 24> m_poses;
    static AnimationLayer m_overlays[13];
    std::memcpy(&m_overlays, local->get_animlayers(), sizeof(AnimationLayer) * 13);
    m_poses = local->m_flPoseParameter();
    static float proper_abs = state->m_flGoalFeetYaw;
    static std::array<float, 24> sent_pose_params = local->m_flPoseParameter();
    static std::array< AnimationLayer, 13 > o_layers;

    local->update_animation_state(state, game::main.get_command()->viewangles);
    std::memcpy(&o_layers, local->get_animlayers(), sizeof(AnimationLayer) * 13);

    if (game::main.send_packet)
    {
        m_server_abs_rotation = state->m_flGoalFeetYaw;
        proper_abs = state->m_flGoalFeetYaw;
        sent_pose_params = local->m_flPoseParameter();
    }
    
    local->set_abs_angles(Vector(0, proper_abs, 0));
    
    std::memcpy(local->get_animlayers(), &o_layers, sizeof(AnimationLayer) * 13);
    std::memcpy(local->get_animlayers(), &m_overlays, sizeof(AnimationLayer) * 13);
    local->m_flPoseParameter() = m_poses;
    local->m_flPoseParameter() = sent_pose_params;

    // interfaces::global_vars->curtime = ba_cur;
}

void c_animations::player_animations()
{
    if (!game::main.get_local())
        return;

    for (auto idx = 1; idx < interfaces::global_vars->maxClients; idx++)
    {
        auto player = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(idx));

        auto& data = m_player_states.at(idx);

        if (!player || player->IsDormant() || !player->is_alive())
        {
            data.m_init = false;
            continue;
        }

        if (player->EntIndex() == interfaces::engine->GetLocalPlayer())
            continue;

        if (data.m_spawntime != player->m_flSpawnTime() || m_should_update_entity_animstate)
        {
            data.m_init = false;
            data.m_spawntime = player->m_flSpawnTime();
        }

        if (!data.m_state)
            data.m_init = false;

        if (!data.m_init)
        {
            data.m_state = static_cast<CCSGOPlayerAnimState*> (interfaces::mem_alloc->alloc(sizeof(CCSGOPlayerAnimState)));

            if (data.m_state != nullptr)
                player->create_animation_state(data.m_state);

            data.m_init = true;
        }

        if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
            *reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

        if (data.m_last_simtime == player->m_flSimulationTime())
        {
            player->update_animation_state(data.m_state, Vector(player->m_angEyeAngles().x,
                player->m_angEyeAngles().y, 0.f));

            if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
                *reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;
        }
        else
        {
            player->update_animation_state(data.m_state, player->m_angEyeAngles());

            if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
                *reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

            player->invalidate_bone_cache();
            player->set_abs_angles(Vector(0, reinterpret_cast<CCSGOPlayerAnimState*> (data.m_state)->m_flGoalFeetYaw, 0));

            data.m_should_recalculate_matrix = true;

            data.m_got_matrix = player->SetupBones(data.m_matrix, 128, BONE_USED_BY_ANYTHING, 0.f);

            data.m_should_recalculate_matrix = false;
        }

        data.m_last_simtime = player->m_flSimulationTime();
    }

    m_should_update_entity_animstate = false;
}

