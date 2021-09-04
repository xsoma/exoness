#include "csgo_hooks.h"
#include "draw_manager.h"
#include "menu.h"
#include "utils.h"

namespace hooks 
{
	void load() 
	{

		while (!(hCSGOWindow = FindWindowA("Valve001", nullptr)))
		{
			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(50ms);
		}

		if (hCSGOWindow)
			pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hCSGOWindow, GWLP_WNDPROC,
				reinterpret_cast<LRESULT>(WndProc)));

		client_hook = std::make_unique<vmt>(interfaces::client);
		client_hook->hook(37, hooks::frame_stage_notify);
		client_hook->hook(24, hooks::write_user_cmd);

		surface_hook = std::make_unique<vmt>(interfaces::surface);
		surface_hook->hook(67, hooks::lock_cursor);

		panel_hook = std::make_unique<vmt>(interfaces::panel);
		panel_hook->hook(41, hooks::paint_traverse);

		client_mode_hook = std::make_unique<vmt>(interfaces::clientmode);
		client_mode_hook->hook(24, hooks::create_move);
		client_mode_hook->hook(44, hooks::post_screen_effects);
		client_mode_hook->hook(18, hooks::over_ride_view);

		direct3d = std::make_unique<vmt>(interfaces::d3d_device9);
		direct3d->hook(42, hooks::end_scene);

		render_view_hook = std::make_unique<vmt>(interfaces::renderview);
		render_view_hook->hook(9, hooks::scene_end);

		model_render = std::make_unique<vmt>(interfaces::modelrender);
		model_render->hook(21, hooks::draw_model_execute);

		bsp_hook = std::make_unique<vmt>(interfaces::engine->GetBSPTreeQuery());
		bsp_hook->hook(6, hooks::list_leaves_in_box);

		trace_ray_hook = std::make_unique<vmt>(interfaces::trace);
		trace_ray_hook->hook(5, hooks::trace_ray);

		prediction_hook = std::make_unique<vmt>(interfaces::prediction);
		prediction_hook->hook(14, hooks::in_prediction);

		engine_hook = std::make_unique<vmt>(interfaces::engine);
		engine_hook->hook(93, hooks::is_hltv);

		event_manager.RegisterSelf();

		render::get().small_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().small_font, "Verdana", 13, 200, 0, 0, FONTFLAG_NONE);

		render::get().small_pixel_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().small_pixel_font, "Smallest pixel-7", 11, 100, 0, 0, FONTFLAG_OUTLINE);

		render::get().cool_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().cool_font, "Gang of Three", 70, 0, 0, 0, FONTFLAG_ANTIALIAS);

		render::get().alternative_pixel_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().alternative_pixel_font, "Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE);

		render::get().visuals_font_name = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().visuals_font_name, "Verdana", 12, 500, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

		render::get().visuals_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().visuals_font, "Small Fonts", 8, 500, 0, 0, FONTFLAG_OUTLINE);

		render::get().no_shadow = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().no_shadow, "Tahoma", 12, 500, 0, 0, FONTFLAG_DROPSHADOW);

		render::get().console_font = interfaces::surface->CreateFont_();
		interfaces::surface->SetFontGlyphSet(render::get().console_font, "Lucida Console", 10, 0, 0, 0, FONTFLAG_DROPSHADOW);
	}
	
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
		{
			if (wParam != vKey) return;

			if (uMsg == WM_KEYDOWN)
				bButton = true;
			else if (uMsg == WM_KEYUP)
				bButton = false;
		};

		const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
		{
			if (wParam != vKey) return;

			if (uMsg == WM_KEYUP)
				bButton = !bButton;
		};

		if (bInitializedDrawManager)
		{
			if (old_ui::get().menuOpened)
				return true;
			
		}

		return CallWindowProcA(pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
	}

	std::unique_ptr <vmt> client_hook;
	std::unique_ptr <vmt> engine_hook;
	std::unique_ptr <vmt> panel_hook;
	std::unique_ptr <vmt> surface_hook;
	std::unique_ptr <vmt> client_mode_hook;
	std::unique_ptr <vmt> direct3d;
	std::unique_ptr <vmt> model_render;
	std::unique_ptr <vmt> bsp_hook;
	std::unique_ptr <vmt> render_view_hook;
	std::unique_ptr <vmt> trace_ray_hook;
	std::unique_ptr <vmt> prediction_hook;

	setup_bones_t original_setupbones;

	c_hook_events event_manager;

	HWND hCSGOWindow = nullptr;
	bool bInitializedDrawManager = false;
	WNDPROC pOriginalWNDProc = nullptr;
}