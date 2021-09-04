#include "chams.h"
#include "main_entity.h"
#include "utils.h"
#include "key_values.h"
#include "animations.h"
#include "menu.h"

using on_render_exec = void(__thiscall*) (IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

IMaterial* create_material(bool shade) {
    static const std::string material_name = "glowOverlay.vmt";
    const std::string material_type = shade ? "VertexLitGeneric" : "UnlitGeneric";

    std::string material_data;

    material_data += "\"" + material_type + "\"\n{\n";
    material_data += "\t\"$additive\" \"1\"\n";
    material_data += "\t\"$envmap\" \"models/effects/cube_white\"\n";
    material_data += "\t\"$envmapfresnel\" \"1\"\n";
    material_data += "\t\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n";
    material_data += "\t\"$alpha\" \"1.0\"\n";
    material_data += "}\n";

    auto init_key_values = [](KeyValues* keyvalues, const char* key_name) -> void {
        using InitKeyValues_t = void(__thiscall*)(void*, const char*);
        static InitKeyValues_t InitKeyValuesEx = nullptr;
        static auto pThis = *reinterpret_cast<DWORD**>(utils::find_signature("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
        if (!InitKeyValuesEx)
            InitKeyValuesEx = *reinterpret_cast<InitKeyValues_t>(utils::find_signature("client.dll", "55 8B EC 51 33 C0 C7 45"));

        InitKeyValuesEx(keyvalues, key_name);
    };
    auto load_from_buffer = [](KeyValues* key_values, const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr) -> void {
        using LoadFromBuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
        static LoadFromBuffer_t LoadFromBufferEx = nullptr;

        if (!LoadFromBufferEx)
            LoadFromBufferEx = *reinterpret_cast<LoadFromBuffer_t>(utils::find_signature("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));

        LoadFromBufferEx(key_values, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
    };

    KeyValues* key_values = (KeyValues*)malloc(sizeof(KeyValues));
    init_key_values(key_values, material_type.c_str());
    load_from_buffer(key_values, material_name.c_str(), material_data.c_str());

    return interfaces::materialsystem->CreateMaterial(material_name.c_str(), key_values);
}

void c_chams::over_ride_material(bool ignoreZ, bool wireframe, bool use_env, const Color& rgba, IMaterial* material, int alpha) {
   
    material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignoreZ);
    material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
    material->IncrementReferenceCount();

    if (alpha == 255)
        material->AlphaModulate(255.0f / alpha);
    else
        material->AlphaModulate(alpha * 0.01f);

    if (!use_env)   
        material->ColorModulate(rgba.red / 255.0f, rgba.green / 255.0f, rgba.blue / 255.0f);
    else
    {
        bool bFound = false;
        IMaterialVar* pMatVar = material->FindVar("$envmaptint", &bFound);
        if (bFound)
            (*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pMatVar + 0x2c))((uintptr_t)pMatVar, rgba.red / 255.0f, rgba.green / 255.0f, rgba.blue / 255.0f);
    }

    interfaces::modelrender->ForcedMaterialOverride(material);
}

void c_chams::on_render(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* matrix)
{
	static auto ofunc = hooks::model_render->get_original< on_render_exec >(21);

    auto entity = reinterpret_cast<c_base_entity*> (interfaces::entitylist->GetClientEntity(pinfo.entity_index));

    const char* ModelName = interfaces::modelinfo->GetModelName((model_t*)pinfo.pModel);

    bool is_player = strstr(ModelName, "models/player") != nullptr;

    if (!entity || entity->IsDormant() || !game::main.get_local() || !matrix)
        return ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);

    auto local = reinterpret_cast<c_base_entity*> (game::main.get_local());

    static IMaterial* test_mat;

    static std::vector< IMaterial* > materials;
    static std::vector< IMaterial* > materials_team;

    int current_mat = 0;
    int current_mat_t = 0;
    int current_mat_t_local = 0;
    int current_mat_t_fake = 0;

    static bool once{ false };

    if (!once) {
        materials.push_back(interfaces::materialsystem->FindMaterial("debug/debugambientcube"));
        materials.push_back(interfaces::materialsystem->FindMaterial("debug/debugdrawflat"));
        materials.push_back(interfaces::materialsystem->FindMaterial("dev/glow_armsrace.vmt", nullptr));
        materials.push_back(create_material(true));
        once = true;
    }

    bool is_glow = c_config::get().chams_mode_enemy == 3 || c_config::get().chams_mode_enemy == 4;
    bool is_glow_team = c_config::get().chams_mode_teammate == 3 || c_config::get().chams_mode_teammate == 4;
    bool is_glow_local = c_config::get().chams_mode_local == 3 || c_config::get().chams_mode_local == 4;
    bool is_glow_fake = c_config::get().chams_mode_fake == 3 || c_config::get().chams_mode_fake == 4;

    switch (c_config::get().chams_mode_fake)
    {
    case 1:
        current_mat_t_fake = 0;
        break;
    case 2:
        current_mat_t_fake = 1;
        break;
    case 3:
        current_mat_t_fake = 2;
        break;
    case 4:
        current_mat_t_fake = 3;
        break;
    }

    switch (c_config::get().chams_mode_local)
    {
    case 1:
        current_mat_t_local = 0;
        break;
    case 2:
        current_mat_t_local = 1;
        break;
    case 3:
        current_mat_t_local = 2;
        break;
    case 4:
        current_mat_t_local = 3;
        break;
    }

    switch (c_config::get().chams_mode_teammate)
    {
    case 1:
        current_mat_t = 0;
        break;
    case 2:
        current_mat_t = 1;
        break;
    case 3:
        current_mat_t = 2;
        break;
    case 4:
        current_mat_t = 3;
        break;
    }

    switch (c_config::get().chams_mode_enemy)
    {
    case 1:
        current_mat = 0;
        break;
    case 2:
        current_mat = 1;
        break;
    case 3:
        current_mat = 2;
        break;
    case 4:
        current_mat = 3;
        break;
    }

    if (c_config::get().chams_fake)
    {
        if (is_player && entity->is_player() && entity == local)
        {
            //if (c_animations::get().m_got_fake_matrix)
           // {
            if (c_config::get().interpolate)
            {
                for (auto& i : c_animations::get().m_fake_matrix)
                {
                    i[0][3] += pinfo.origin.x;
                    i[1][3] += pinfo.origin.y;
                    i[2][3] += pinfo.origin.z;
                }
            }
                if (c_config::get().gloss_fake && is_glow_fake)
                {
                    over_ride_material(false, c_config::get().second_wireframe_fake, false, Color(c_config::get().gloss_color_fake_r, c_config::get().gloss_color_fake_g, c_config::get().gloss_color_fake_b), materials.at(0), c_config::get().transparency_second_fake);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, c_animations::get().m_fake_matrix);
                }

                over_ride_material(false, c_config::get().wireframe_fake, is_glow_fake ? true : false, Color(c_config::get().chams_color_fake_r, c_config::get().chams_color_fake_g, c_config::get().chams_color_fake_b), materials.at(current_mat_t_fake), c_config::get().transparency_fake);
                ofunc(interfaces::modelrender, ctx, state, pinfo, c_animations::get().m_fake_matrix);

                interfaces::modelrender->ForcedMaterialOverride(nullptr);

                if (c_config::get().interpolate)
                {
                    for (auto& i : c_animations::get().m_fake_matrix)
                    {
                        i[0][3] -= pinfo.origin.x;
                        i[1][3] -= pinfo.origin.y;
                        i[2][3] -= pinfo.origin.z;
                    }
                }
            //}
        }
    }

    if (is_player && entity->is_player())
    {
        const auto player = dynamic_cast<c_base_entity*> (entity);

        if (!player)    
            return;

        if (player->is_alive() && player->m_iTeamNum() != game::main.get_local()->m_iTeamNum() && player != game::main.get_local())
        {
            if (c_config::get().chams_mode_enemy > 0)
            {
                if (c_config::get().chams_enemies && c_config::get().chams_xqz_enemies)
                {
                    if (c_config::get().gloss_effect && is_glow)
                    {
                        over_ride_material(true, c_config::get().wireframe_second, false, Color(c_config::get().gloss_effect_r_xqz, c_config::get().gloss_effect_g_xqz, c_config::get().gloss_effect_b_xqz), materials.at(0), c_config::get().transparency_second);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                        over_ride_material(false, c_config::get().wireframe_second, false, Color(c_config::get().gloss_effect_r, c_config::get().gloss_effect_g, c_config::get().gloss_effect_b), materials.at(0), c_config::get().transparency_second);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    }
                    
                    over_ride_material(true, c_config::get().wireframe, is_glow ? true : false, Color(c_config::get().chams_xqz_esp_color_r, c_config::get().chams_xqz_esp_color_g, c_config::get().chams_xqz_esp_color_b), materials.at(current_mat), c_config::get().transparency);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    over_ride_material(false, c_config::get().wireframe, is_glow ? true : false, Color(c_config::get().chams_esp_color_r, c_config::get().chams_esp_color_g, c_config::get().chams_esp_color_b), materials.at(current_mat), c_config::get().transparency);

                }
                else if (c_config::get().chams_enemies && !c_config::get().chams_xqz_enemies)
                {
                    if (c_config::get().gloss_effect && is_glow)
                    {
                        over_ride_material(false, c_config::get().wireframe_second, false, Color(c_config::get().gloss_effect_r, c_config::get().gloss_effect_g, c_config::get().gloss_effect_b), materials.at(0), c_config::get().transparency_second);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    }

                    over_ride_material(false, c_config::get().wireframe, is_glow ? true : false, Color(c_config::get().chams_esp_color_r, c_config::get().chams_esp_color_g, c_config::get().chams_esp_color_b), materials.at(current_mat), c_config::get().transparency);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);

                }
            }
        }
        else if (player->is_alive() && player->m_iTeamNum() == game::main.get_local()->m_iTeamNum() && player != game::main.get_local())
        {
            if (c_config::get().chams_mode_teammate > 0)
            {
                if (c_config::get().chams_team && c_config::get().c_config::get().chams_xqz_team)
                {
                    if (c_config::get().gloss_effect_team && is_glow_team)
                    {
                        over_ride_material(true, c_config::get().wireframe_second_team, false, Color(c_config::get().gloss_effect_r_xqz_team, c_config::get().gloss_effect_g_xqz_team, c_config::get().gloss_effect_b_xqz_team), materials.at(0), c_config::get().transparency_second_team);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                        over_ride_material(false, c_config::get().wireframe_second_team, false, Color(c_config::get().gloss_effect_r_team, c_config::get().gloss_effect_g_team, c_config::get().gloss_effect_b_team), materials.at(0), c_config::get().transparency_second_team);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    }

                    over_ride_material(true, c_config::get().wireframe_team, is_glow_team ? true : false, Color(c_config::get().chams_xqz_esp_color_r_team, c_config::get().chams_xqz_esp_color_g_team, c_config::get().chams_xqz_esp_color_b_team), materials.at(current_mat_t), c_config::get().transparency_team);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    over_ride_material(false, c_config::get().wireframe_team, is_glow_team ? true : false, Color(c_config::get().chams_esp_color_r_team, c_config::get().chams_esp_color_g_team, c_config::get().chams_esp_color_b_team), materials.at(current_mat_t), c_config::get().transparency_team);

                }
                else if (c_config::get().chams_team && !c_config::get().chams_xqz_team)
                {
                    if (c_config::get().gloss_effect_team && is_glow_team)
                    {
                        over_ride_material(false, c_config::get().wireframe_second_team, false, Color(c_config::get().gloss_effect_r_team, c_config::get().gloss_effect_g_team, c_config::get().gloss_effect_b_team), materials.at(0), c_config::get().transparency_second_team);
                        ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                    }

                    over_ride_material(false, c_config::get().wireframe_team, is_glow_team ? true : false, Color(c_config::get().chams_esp_color_r_team, c_config::get().chams_esp_color_g_team, c_config::get().chams_esp_color_b_team), materials.at(current_mat_t), c_config::get().transparency_team);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);

                }
            }
        }
        else if (player->is_alive() && player == game::main.get_local())
        {
            if (c_config::get().chams_local)
            {
                if (c_config::get().gloss_local && is_glow_local)
                {
                    over_ride_material(false, c_config::get().second_wireframe_local, false, Color(c_config::get().gloss_color_local_r, c_config::get().gloss_color_local_g, c_config::get().gloss_color_local_b), materials.at(0), c_config::get().transparency_second_local);
                    ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
                }

                over_ride_material(false, c_config::get().wireframe_local, is_glow_local ? true : false, Color(c_config::get().chams_color_local_r, c_config::get().chams_color_local_g, c_config::get().chams_color_local_b), materials.at(current_mat_t_local), c_config::get().transparency_local);
                ofunc(interfaces::modelrender, ctx, state, pinfo, matrix);
            }
        }
    }
}