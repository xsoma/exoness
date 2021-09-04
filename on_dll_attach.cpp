#include <Windows.h>
#include <iostream>
#include <thread>
#include "csgo_hooks.h"
#include "net_var_manager.h"
#include "csgo_alt.h"

DWORD WINAPI dll_setup(HINSTANCE module_handle) {

	interfaces::init();
	NetvarSys::get().Initialize();
	hooks::load();
	detour_hook::init();

	while (true) {

		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(1s);
	}

}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason_for_call, LPVOID reserved) {

	switch (reason_for_call) {

	case DLL_PROCESS_ATTACH:

		DisableThreadLibraryCalls(module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dll_setup, NULL, NULL, NULL);
		break;

	}

	return TRUE;

}