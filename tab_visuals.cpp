#include "menu.h"

void old_ui::draw_visuals_tab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width + 1;
	int correct = 0;
	Color theme = Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main);
	static std::string ESP_Player_SubTabs_Names[4] = { "main", "chams", "other", "other ext." };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = setuped->text_size_to_rect(setuped->main_font, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 54 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 11,
			(in_sizew_esp_player_subtabs / tab_amount), 24
		};

		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			current_players_esp_subtab = i;

		setuped->vertical_gradient_2(tab_area[0], tab_area[1], (in_sizew_esp_player_subtabs / tab_amount) + 1, tab_area[3], Color(23, 23, 23), Color(15, 15, 15));

		if (current_players_esp_subtab == i)
		{
			setuped->rect_(tab_area[0] - 1, tab_area[1], tab_area[2] + 3, tab_area[3], Color(0, 0, 0, MenuAlpha_Main));

			setuped->text(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2) - 1, setuped->main_font, ESP_Player_SubTabs_Names[i].c_str(), Color(200, 200, 200, MenuAlpha_Main));

			setuped->rect_fill_(tab_area[0] - 1, tab_area[1] + 23, tab_area[2] + 3, 1, theme);
		}
		else
		{
			setuped->rect_(tab_area[0] - 1, tab_area[1], tab_area[2] + 3, tab_area[3], Color(0, 0, 0, MenuAlpha_Main));

			setuped->text(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2) - 1, setuped->main_font, ESP_Player_SubTabs_Names[i].c_str(), Color(42, 42, 42, MenuAlpha_Main));

			setuped->rect_fill_(tab_area[0] - 1, tab_area[1] + 23, tab_area[2] + 3, 1, Color(29, 29, 29, MenuAlpha_Main));
		}
	}
}