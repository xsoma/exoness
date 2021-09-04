#pragma once

#define NOMINMAX

#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>

#include "e_nums.h"
#include "virtual_func.h"

#include "v_matrix.h"
#include "q_angle.h"
#include "vector.h"
#include "studio.h"

#include "i_alloc.h"
#include "i_app_system.h"
#include "i_base_client_dll.h"
#include "i_client_entity.h"
#include "i_client_entity_list.h"
#include "i_client_mode.h"
#include "i_con_var.h"
#include "i_cvar.h"
#include "i_engine_trace.h"
#include "i_v_engine_client.h"
#include "i_v_debug_overlay.h"
#include "i_surface.h"
#include "i_input.h"
#include "i_v_model_info_client.h"
#include "i_v_model_render.h"
#include "i_render_view.h"
#include "i_game_event_manager.h"
#include "i_material_system.h"
#include "i_move_helper.h"
#include "i_mdl_cache.h"
#include "i_prediction.h"
#include "i_panel.h"
#include "i_view_render.h"
#include "i_client_state.h"
#include "i_physics.h"
#include "i_input_system.h"
#include "i_glow_manager.h"

#include "c_convar.h"
#include "c_user_cmd.h"
#include "data_map.h"

#include "singleton.h"

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define GETBITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GETBYTE( x )    (GETBITS(x[0]) << 4 | GETBITS(x[1]))

class InterfaceReg {
private:
	using InstantiateInterfaceFn = void* (*)();
public:
	InstantiateInterfaceFn m_CreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;
};

struct IDirect3DDevice9;

namespace interfaces
{
	extern IVEngineClient* engine;
	extern IInputSystem* inputsys;
	extern IBaseClientDLL* client;
	extern IClientEntityList* entitylist;
	extern ICvar* cvar;
	extern IEngineTrace* trace;
	extern DWORD* inputinternal;
	extern 	IVRenderView* renderview;
	extern IMDLCache* modelcache;
	extern IVModelRender* modelrender;
	extern IMaterialSystem* materialsystem;
	extern IPanel* panel;
	extern IVModelInfoClient* modelinfo;
	extern IPrediction* prediction;
	extern IGameMovement* gamemovement;
	extern ISurface* surface;
	extern IVDebugOverlay* debugoverlay;
	extern IPhysicsSurfaceProps* physsurface;
	extern IGameEventManager2* eventmanager;
	extern IClientMode* clientmode;
	extern CGlobalVarsBase* global_vars;
	extern IDirect3DDevice9* d3d_device9;
	extern CGlowObjectManager* glow;
	extern CClientState* clientstate;
	extern IMoveHelper* movehelper;
	extern CInput* input;
	extern IMemAlloc* mem_alloc;
	extern DWORD postprocessing;
	extern DWORD ccsplayerrenderablevftable;

	void init( );

	template<typename T>
	T* get(const char* mod_name, const char* interface_name, bool exact = false) {
		T* iface = nullptr;
		InterfaceReg* register_list;
		int part_match_len = strlen(interface_name);

		DWORD interface_fn = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandleA(mod_name), "CreateInterface"));

		if (!interface_fn) {
			return nullptr;
		}

		unsigned int jump_start = (unsigned int)(interface_fn)+4;
		unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

		register_list = **reinterpret_cast<InterfaceReg***>(jump_target + 6);

		for (InterfaceReg* cur = register_list; cur; cur = cur->m_pNext) {
			if (exact == true) {
				if (strcmp(cur->m_pName, interface_name) == 0)
					iface = reinterpret_cast<T*>(cur->m_CreateFn());
			}
			else {
				if (!strncmp(cur->m_pName, interface_name, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0)
					iface = reinterpret_cast<T*>(cur->m_CreateFn());
			}
		}
		return iface;
	}

}