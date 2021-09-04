#include "csgo_alt.h"
#include "main_entity.h"

bool is_local_player()
{
	return interfaces::entitylist->GetClientEntity(interfaces::engine->GetLocalPlayer());
}

detour_hook::update_anims_fn detour_hook::original_update;

void detour_hook::should_setup_bones(void* ecx, void* edx)
{
	// del
}