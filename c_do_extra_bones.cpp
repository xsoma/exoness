#include "csgo_alt.h"

detour_hook::do_extra_bone_processing_t detour_hook::orig_do_extra_bone_processing;

void _fastcall detour_hook::do_extra_bone_processing(void* ecx, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context)
{
	auto player = (c_base_entity*)ecx;

	if (player->m_fEffects() & 8)
		return;

	const auto state = uint32_t(player->get_animation_state());

	if (!state)
		return orig_do_extra_bone_processing(player, hdr, pos, q, matrix, bone_computed, context);

	const auto backup_tickcount = *reinterpret_cast<int32_t*>(state + 8);
	*reinterpret_cast<int32_t*>(state + 8) = 0;
	orig_do_extra_bone_processing(player, hdr, pos, q, matrix, bone_computed, context);
	*reinterpret_cast<int32_t*>(state + 8) = backup_tickcount;
}