#include "i_interfaces.h"

#define tick_interval			(interfaces::global_vars->interval_per_tick)
#define ticks_to_time(t) (interfaces::global_vars->interval_per_tick * (t) )
#define time_to_ticks( dt )		( (int)( 0.5 + (float)(dt) / interfaces::global_vars->interval_per_tick ) )

namespace utils
{
	int epoch_time();
	unsigned long pattern_scan(const char* module_name, const char* signature);
	uintptr_t find_signature(const char* szModule, const char* szSignature);
	bool world_to_screen(const Vector& origin, Vector2D& screen);
	DWORD find_by_pattern(const char* module_name, const BYTE* mask, const char* mask_string);
	bool world_to_screen_vector(const Vector& origin, Vector& screen);
	void trace_line(const Vector& vec_start, const Vector& vec_end, const unsigned int n_mask, c_base_entity* p_cs_ignore, trace_t* p_trace);
	float get_estimate_server_time(CUserCmd* cmd);
	void fix_movement(Vector& oldang, CUserCmd* cmd);
	bool is_any_visible(const Vector& start, const Vector& end, c_base_entity* player, c_base_entity* local);
	void clip_ray_to_player(const Vector& src, const Vector& end, const uint32_t mask, c_base_entity* player, ITraceFilter* filter, trace_t* t);
	bool is_visible(Vector pos_notangles, bool smokecheck);
	bool method_key_bind(bool hold, int key);
}





































