#include "i_interfaces.h"
#include "utils.h"

namespace interfaces
{
	IVEngineClient* engine = nullptr;
	IInputSystem* inputsys = nullptr;
	IBaseClientDLL* client = nullptr;
	IClientEntityList* entitylist = nullptr;
	ICvar* cvar = nullptr;
	IEngineTrace* trace = nullptr;
	DWORD* inputinternal = nullptr;
	IVRenderView* renderview = nullptr;
	IMDLCache* modelcache = nullptr;
	IVModelRender* modelrender = nullptr;
	IMaterialSystem* materialsystem = nullptr;
	IPanel* panel = nullptr;
	IVModelInfoClient* modelinfo = nullptr;
	IPrediction* prediction = nullptr;
	IGameMovement* gamemovement = nullptr;
	ISurface* surface = nullptr;
	IVDebugOverlay* debugoverlay = nullptr;
	IPhysicsSurfaceProps* physsurface = nullptr;
	IGameEventManager2* eventmanager = nullptr;
	IClientMode* clientmode = nullptr;
	CGlobalVarsBase* global_vars = nullptr;
	IDirect3DDevice9* d3d_device9 = nullptr;
	CGlowObjectManager* glow = nullptr;
	CClientState* clientstate = nullptr;
	IMoveHelper* movehelper = nullptr;
	CInput* input = nullptr;
	IMemAlloc* mem_alloc = nullptr;
	DWORD postprocessing;
	DWORD ccsplayerrenderablevftable;

	void interfaces::init()
	{
		engine = interfaces::get<IVEngineClient>("engine.dll", "VEngineClient");
		inputsys = interfaces::get<IInputSystem>("inputsystem.dll", "InputSystemVersion");
		client = interfaces::get<IBaseClientDLL>("client.dll", "VClient0");
		entitylist = interfaces::get<IClientEntityList>("client.dll", "VClientEntityList");
		cvar = interfaces::get<ICvar>("vstdlib.dll", "VEngineCvar");
		trace = interfaces::get<IEngineTrace>("engine.dll", "EngineTraceClient");
		inputinternal = interfaces::get<DWORD>("vgui2.dll", "VGUI_InputInternal");
		renderview = interfaces::get<IVRenderView>("engine.dll", "VEngineRenderView");
		modelcache = interfaces::get<IMDLCache>("datacache.dll", "MDLCache");
		modelrender = interfaces::get<IVModelRender>("engine.dll", "VEngineModel");
		materialsystem = interfaces::get<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		panel = interfaces::get<IPanel>("vgui2.dll", "VGUI_Panel");
		modelinfo = interfaces::get<IVModelInfoClient>("engine.dll", "VModelInfoClient");
		prediction = interfaces::get<IPrediction>("client.dll", "VClientPrediction");
		gamemovement = interfaces::get<IGameMovement>("client.dll", "GameMovement");
		surface = interfaces::get<ISurface>("vguimatsurface.dll", "VGUI_Surface");
		debugoverlay = interfaces::get<IVDebugOverlay>("engine.dll", "VDebugOverlay");
		physsurface = interfaces::get< IPhysicsSurfaceProps >("vphysics.dll", "VPhysicsSurfaceProps");
		eventmanager = interfaces::get<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002", true);
		clientmode = **reinterpret_cast<IClientMode***>((*(DWORD**)(client))[10] + 0x5);
		global_vars = **reinterpret_cast<CGlobalVarsBase***>((*(DWORD**)(client))[0] + 0x1F);
		d3d_device9 = **reinterpret_cast<IDirect3DDevice9***> (utils::pattern_scan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		glow = *reinterpret_cast<CGlowObjectManager**>(utils::pattern_scan("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01") + 0x3);
		clientstate = **reinterpret_cast<CClientState***> ((*reinterpret_cast<uintptr_t**> (engine))[12] + 0x10);
		mem_alloc = *reinterpret_cast<IMemAlloc**> (GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));
		movehelper = **reinterpret_cast<IMoveHelper***>(utils::pattern_scan("client.dll", "8B 0D ?? ?? ?? ?? 8B 46 08 68") + 0x2);
		input = *reinterpret_cast<CInput**>(utils::pattern_scan("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
		postprocessing = *(DWORD*)(utils::pattern_scan("client.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 2);
		ccsplayerrenderablevftable = *(DWORD*)(utils::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x4E);
	}

}