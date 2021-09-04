#include "chams.h"
#include "main_entity.h"
#include "animations.h"

using draw_model_execute_t = void(__thiscall*) (IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

void __stdcall hooks::draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* custom_bone_to_world)
{
    static auto ofunc = hooks::model_render->get_original< draw_model_execute_t >(21);

    if (!custom_bone_to_world || !interfaces::renderview)
        return ofunc(interfaces::modelrender, ctx, state, pinfo, custom_bone_to_world);

    if (interfaces::modelrender->IsForcedMaterialOverride() && !strstr(pinfo.pModel->szName, "arms") &&!strstr(pinfo.pModel->szName, "weapons/v_"))
        return ofunc(interfaces::modelrender, ctx, state, pinfo, custom_bone_to_world);

    c_chams::get().on_render(ctx, state, pinfo, custom_bone_to_world);

    ofunc(interfaces::modelrender, ctx, state, pinfo, custom_bone_to_world);

    interfaces::modelrender->ForcedMaterialOverride(nullptr);
    interfaces::renderview->SetBlend(1.f);
    
}