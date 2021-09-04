#pragma once
#include "csgo_hooks.h"

class c_chams : public singleton <c_chams>
{

public:

	void on_render ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* matrix );

private:

	void over_ride_material ( bool ignoreZ, bool wireframe, bool use_env, const Color& rgba, IMaterial* material, int alpha );

};