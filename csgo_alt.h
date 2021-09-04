#include "i_interfaces.h"
#include "singleton.h"

#include "detours.h"
#include "main_entity.h"

#include <chrono>
#include <thread>
#include <assert.h>

#include <thread>
#include <Psapi.h>

typedef float quaternion_t[4];

namespace detour_hook
{
	void init ( );

	typedef bool(__thiscall* SetupBones_t)(IClientRenderable*, matrix3x4_t*, int, int, float);
	extern SetupBones_t original_setup_bones;
	bool __fastcall setup_bones(IClientRenderable* ecx, void* edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float curtime);

	typedef void(__thiscall* standard_blending_rules_t)(c_base_entity*, studiohdr_t*, Vector*, Quaternion*, float, int);
	extern standard_blending_rules_t origin_standard_blending_rules;
	void _fastcall standard_blending_rules(c_base_entity* player, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const float time, int mask);

	typedef void(__thiscall* do_extra_bone_processing_t)(void*, studiohdr_t*, Vector*, Quaternion*, const matrix3x4_t&, uint8_t*, void*);
	extern do_extra_bone_processing_t orig_do_extra_bone_processing;
	void _fastcall  do_extra_bone_processing(void* ecx, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context);

	using accumulate_layers_fn = void(__thiscall*)(c_base_entity * player, uintptr_t edx, void* setup, Vector * pos, float time, quaternion_t * q);
	extern accumulate_layers_fn orig_accumulate_layers;
	void __fastcall accumulate_layers(c_base_entity* player, uintptr_t edx, void* setup, Vector* pos, float time, quaternion_t* q);

	typedef void(__thiscall* build_transformations_t)(c_base_entity*, studiohdr_t*, Vector*, Quaternion*, const matrix3x4_t&, const int32_t, byte*);
	extern build_transformations_t original_build_transformations;
	void __fastcall build_transformations(c_base_entity* player, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& transform, const int32_t mask, byte* computed);

	typedef void(__thiscall* update_client_side_animation_t)(c_base_entity*);
	extern update_client_side_animation_t origin_update_client_side_animation;
	void __fastcall update_client_side_animation(c_base_entity* player, uint32_t);

	using update_anims_fn = void(__thiscall*)(void*);
	extern update_anims_fn original_update;
	void should_setup_bones(void* ecx, void* edx);
}