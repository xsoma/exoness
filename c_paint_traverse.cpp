#include "csgo_hooks.h"
#include "menu.h"
#include "esp.h"
#include "world.h"
#include "animations.h"
#include "logs.h"

using PaintTraverse_t = void(__thiscall*)(void*, vgui::VPANEL, bool, bool);

void __fastcall hooks::paint_traverse(void* ecx, void* edx, vgui::VPANEL panel, bool force_repaint, bool allow_force)
{
	static auto o_paint_traverse = hooks::panel_hook->get_original<PaintTraverse_t>(41);
	static unsigned int panel_hud_id, panel_id;

	if (!panel_hud_id)
		if (!strcmp("HudZoom", interfaces::panel->GetName(panel)))
			panel_hud_id = panel;

	if (c_config::get().remove[0] && panel_hud_id == panel && game::main.get_local())
		if (game::main.get_local()->m_bIsScoped())
			return;

	o_paint_traverse(ecx, panel, force_repaint, allow_force);

	if (!panel_id)
		if (!strcmp("MatSystemTopPanel", interfaces::panel->GetName(panel)))
			panel_id = panel;

	if (panel_id == panel)
	{
		if (game::main.get_local())
		{
			if (interfaces::engine->IsInGame() || interfaces::engine->IsConnected())
			{
				if (c_config::get().remove[0])
					esp::get().on_render_scope();

				if (c_config::get().visuals_enabled || c_config::get().visuals_world_enabled)
					esp::get().on_render();

				if (c_config::get().visuals_world_enabled && c_config::get().radius)
					esp::get().on_render_molotov();

				logs_put::get().run();
			}
		}

		if (!interfaces::engine->IsInGame())
		{
			c_animations::get().m_should_update_fake = true;
			c_animations::get().m_should_update_real = true;
			c_animations::get().m_should_update_entity_animstate = true;
		}
		
		// old_ui::get().on_render();
	}
}