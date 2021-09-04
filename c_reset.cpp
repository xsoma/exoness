#include "csgo_hooks.h"
#include <intrin.h>
#include "menu.h"
#include "frame.h"

#include "esp.h"
#include "world.h"
#include "logs.h"

using end_scene_fn = long(__stdcall*)(IDirect3DDevice9 * device);

HRESULT __stdcall hooks::end_scene(IDirect3DDevice9* pDevice)
{
    static end_scene_fn original = hooks::direct3d->get_original<end_scene_fn>(42);

    if (setuped == NULL)
    {
        setuped = new d3d_render(pDevice);
        D3DXCreateFont(pDevice, 13, 0, 200, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, "Verdana", &setuped->main_font);
        D3DXCreateFont(pDevice, 11, 0, 100, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, "Smallest pixel-7", &setuped->pixel_font);
        D3DXCreateFont(pDevice, 70, 0, 0, 4, FALSE, DEFAULT_CHARSET, 1, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Gang of Three", &setuped->gang);
        D3DXCreateFont(pDevice, 10, 0, 0, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, "Lucida Console", &setuped->console_font);
        D3DXCreateFont(pDevice, 12, 0, 400, 4, FALSE, DEFAULT_CHARSET, 1, DEFAULT_QUALITY, DEFAULT_PITCH, "Tahoma", &setuped->visualz_font);
        return original(pDevice);
    }

    setuped->save_state();
    setuped->setup_render_states();

    old_ui::get().on_render(pDevice);

    setuped->restore_state();

    return original(pDevice);
}

/*

using p_reset = long(__stdcall*) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);


long __stdcall hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    auto ofunc = hooks::direct3d->get_original< p_reset >(16);

    if (render::get().initalized)
    {
        render::get().invalidate_objects();
        long hr = ofunc(device, pPresentationParameters);
        render::get().restore_objects(device);

        return hr;
    }

    return ofunc(device, pPresentationParameters);
}

using p_present = long(__stdcall*) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);

long __stdcall hooks::present(IDirect3DDevice9* m_device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override,
    const RGNDATA* dirty_region)
{
    static auto original_fn = hooks::direct3d->get_original< p_present >(17);

    [m_device]()
    {
        if (!render::get().initalized)
        {
            render::get().initalize_objects(m_device);
            render::get().initalized = true;
        }
        else
        {
            static const D3DRENDERSTATETYPE backup_states[] = {
                D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE
            };
            static const int size = sizeof(backup_states) / sizeof(DWORD);
            IDirect3DStateBlock9* state_block = nullptr;

            DWORD old_states[size] = { 0 };

            for (int i = 0; i < size; i++)
                m_device->GetRenderState(backup_states[i], &old_states[i]);

            m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
            m_device->CreateStateBlock(D3DSBT_ALL, &state_block);
            state_block->Capture();

            render::get().setup_states();
          
            state_block->Apply();
            state_block->Release();

            /*
            static bool _pressed = true;
            static bool menu_open;

            static bool init = true;

            if (init)
            {
                ui::CreateContext();
                ImGui_ImplDX9_Init(hooks::hCSGOWindow, m_device);
               
                init = false;
            }

            if (!_pressed && GetAsyncKeyState(VK_INSERT))
                _pressed = true;
            else if (_pressed && !GetAsyncKeyState(VK_INSERT))
            {
                _pressed = false;
                menu_open = !menu_open;
            }

            if (menu_open)
            {
                ImGui_ImplDX9_NewFrame();

                ui::SetNextWindowSizeConstraints(ImVec2(300, 500), ImVec2(4096, 4096));

                ui::Begin("color&set", 0, ImGuiWindowFlags_NoTitleBar || ImGuiWindowFlags_NoScrollbar);
                {

                }
                ui::End();
             
            }
            

            for (int i = 0; i < size; i++)
                m_device->SetRenderState(backup_states[i], old_states[i]);
        }
    }
    ();

    return original_fn(m_device, source_rect, dest_rect, dest_window_override, dirty_region);
}
*/
