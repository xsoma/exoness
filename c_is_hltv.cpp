#include "csgo_hooks.h"
#include "utils.h"
#include <intrin.h>

auto ptr_accumulate_layers = reinterpret_cast<void*> (utils::pattern_scan("client.dll", "84 C0 75 0D F6 87"));
auto setupvelocity_call = reinterpret_cast<void*> (utils::pattern_scan(("client.dll"),
("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0")));

using is_hltv_t = bool(__fastcall*) ();

bool __fastcall hooks::is_hltv()
{
    const auto org_f = hooks::engine_hook->get_original< is_hltv_t >(93);

    if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (ptr_accumulate_layers))
        return true;

    if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (setupvelocity_call))
        return true;

    return org_f();
}
