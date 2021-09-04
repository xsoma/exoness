#include "csgo_hooks.h"
#include "globals.h"

using trace_ray_t = void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);

void __fastcall hooks::trace_ray(void* thisptr, void*, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
{
	static auto o_trace_ray = hooks::trace_ray_hook->get_original<trace_ray_t>(5);

	if (!game::main.in_trace)
		return o_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);

	o_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);

	pTrace->surface.flags |= SURF_SKY;
}