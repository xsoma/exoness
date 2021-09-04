#include "csgo_hooks.h"
#include "menu.h"

typedef void(__fastcall* lock_cursor_t) (ISurface*, void*);

void __fastcall hooks::lock_cursor(ISurface* thisptr, void* edx)
{
    static auto oLockCursor = hooks::surface_hook->get_original<lock_cursor_t>(67);

    if (!old_ui::get().menuOpened)
        return oLockCursor(thisptr, edx);

    interfaces::surface->UnlockCursor();
}
