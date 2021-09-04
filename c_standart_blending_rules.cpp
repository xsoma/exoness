#include "csgo_alt.h"
#include "main_entity.h"

static constexpr auto BONE_USED_BY_SERVER = BONE_USED_BY_HITBOX | BONE_USED_BY_VERTEX_LOD0 | BONE_USED_BY_VERTEX_LOD1 | BONE_USED_BY_VERTEX_LOD2
| BONE_USED_BY_VERTEX_LOD3 | BONE_USED_BY_VERTEX_LOD4 | BONE_USED_BY_VERTEX_LOD5 | BONE_USED_BY_VERTEX_LOD6 | BONE_USED_BY_VERTEX_LOD7;

detour_hook::standard_blending_rules_t detour_hook::origin_standard_blending_rules;

void _fastcall detour_hook::standard_blending_rules(c_base_entity* player, uint32_t, studiohdr_t* hdr, Vector* pos, Quaternion* q, const float time, int mask)
{
	auto me = reinterpret_cast<c_base_entity*> ( interfaces::entitylist->GetClientEntity(interfaces::engine->GetLocalPlayer()));

	if (player && player->m_iTeamNum() != me->m_iTeamNum() && player != me)
		mask = BONE_USED_BY_SERVER;

	player->m_fEffects() |= 0x008;
	origin_standard_blending_rules(player, hdr, pos, q, time, mask);
	player->m_fEffects() &= ~0x008;
}