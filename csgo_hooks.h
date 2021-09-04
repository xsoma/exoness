#pragma once

#include "i_interfaces.h"
#include "singleton.h"

#include "vmt.h"

#include <chrono>
#include <thread>
#include <assert.h>

class c_hook_events : public IGameEventListener2 {
public:
	void FireGameEvent(IGameEvent* event);
	void RegisterSelf();
	int GetEventDebugID(void);
};

namespace hooks {

	void load();
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	extern std::unique_ptr <vmt> client_hook;
	extern std::unique_ptr <vmt> panel_hook;
	extern std::unique_ptr <vmt> surface_hook;
	extern std::unique_ptr <vmt> client_mode_hook;
	extern std::unique_ptr <vmt> direct3d;
	extern std::unique_ptr <vmt> model_render;
	extern std::unique_ptr <vmt> bsp_hook;
	extern std::unique_ptr <vmt> render_view_hook;
	extern std::unique_ptr <vmt> prediction_hook;
	extern std::unique_ptr <vmt> trace_ray_hook;
	extern std::unique_ptr <vmt> engine_hook;

	extern c_hook_events event_manager;

	using setup_bones_t = bool(__thiscall*) (void*, matrix3x4_t*, int, int, float);
	extern setup_bones_t original_setupbones;

	extern HWND hCSGOWindow;
	extern bool bInitializedDrawManager;
	extern WNDPROC pOriginalWNDProc;

	bool __stdcall create_move(float, CUserCmd*);
	void __fastcall over_ride_view(void* ecx, void* edx, CViewSetup* pSetup);
	void	__fastcall paint_traverse(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force);
	void __fastcall lock_cursor(ISurface*, void*);
	void __fastcall scene_end(void* ecx, void* edx);
	bool __fastcall in_prediction(void* p);
	void __stdcall draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* custom_bone_to_world);
	long __stdcall present(IDirect3DDevice9* m_device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region);
	long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* p_presentation_parameters);
	void __fastcall post_screen_effects(void* thisptr, void* edx, CViewSetup* setup);
	int __fastcall list_leaves_in_box(std::uintptr_t ecx, std::uintptr_t edx, Vector& mins, Vector& maxs, unsigned short* list, int list_max);
	void __fastcall run_cmd(void* ecx, void*, c_base_entity* player, CUserCmd* ucmd, IMoveHelper* helper);
	void __fastcall trace_ray(void* thisptr, void*, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	bool __fastcall is_hltv();
	HRESULT __stdcall end_scene(IDirect3DDevice9* pDevice);
	bool __fastcall write_user_cmd(void* ECX, void* EDX, int nSlot, bf_write* buf, int from, int to, bool isNewCmd);
	void __stdcall frame_stage_notify(ClientFrameStage_t stage);
}


