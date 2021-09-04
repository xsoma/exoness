#include "csgo_alt.h"

detour_hook::update_client_side_animation_t detour_hook::origin_update_client_side_animation;

void __fastcall detour_hook::update_client_side_animation(c_base_entity* player, uint32_t)
{
	const auto local = reinterpret_cast <c_base_entity*> ( interfaces::entitylist->GetClientEntity(interfaces::engine->GetLocalPlayer()) );

	if (!local || !local->is_alive())
		return origin_update_client_side_animation(player);

	if (game::main.need_to_update)
		origin_update_client_side_animation(player);
}