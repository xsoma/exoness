#include "csgo_alt.h"

detour_hook::build_transformations_t  detour_hook::original_build_transformations;

void __fastcall detour_hook::build_transformations(c_base_entity* player, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const matrix3x4_t& transform, const int32_t mask, byte* computed)
{
	const auto backup_bone_flags = hdr->flags;
	original_build_transformations(player, hdr, pos, q, transform, mask, computed);
	hdr->flags = backup_bone_flags;
}