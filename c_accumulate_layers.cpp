#include "csgo_alt.h"

detour_hook::accumulate_layers_fn detour_hook::orig_accumulate_layers;

void __fastcall detour_hook::accumulate_layers(c_base_entity* player, uintptr_t edx, void* setup, Vector* pos, float time, quaternion_t* q)
{
	*reinterpret_cast<BYTE*> (player + 2600) &= ~0xA;
	orig_accumulate_layers(player, edx, setup, pos, time, q);
	*reinterpret_cast<BYTE*> (player + 2600) |= 0xA;
}