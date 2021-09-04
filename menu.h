#pragma once

#include "draw_manager.h"
#include "config.h"
#include "vector.h"

#include "direct_render.h"

class old_ui : public singleton< old_ui > {

public:

	bool menuOpened;

	void on_render(IDirect3DDevice9* pDevice);

private:

	Vector _pos = Vector(0, 0, 0);

	struct
	{
		float x = 0.f, y = 0.f;
	}Pos;

private:

	int screen_width;
	int screen_height;

	int MenuAlpha_Main;
	int MenuAlpha_Text;

private:

	int GroupBoxSize_Width = 401;
	int GroupBoxSize_Height = 350;

	void draw_aimbot_tab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main);
	void draw_visuals_tab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main);
	void draw_misc_tab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main);

private:

	int y_offset;
	int x_offset;

	int groupbox_width;
	int groupbox_bottom;
	int groupbox_top;

	bool dont_recieve_input = false;

	typedef void(&ButtonCallback_t)(void);

	void button(ButtonCallback_t callback, std::string title);
	void groupbox(int x, int y, int w, int h, std::string name, bool has_subtabs);
	void checkbox(std::string name, bool* item);
	void slider(int max, std::string name, int* item, std::string suffix, int step);
	void slider_float(float max, std::string name, float* item, std::string suffix, float step);
	void color_selector(std::string name, int* red, int* green, int* blue, int* alpha);
	void checkbox_with_picker(std::string name, bool* item);
	void keybind(int* item, std::string name);
	void combobox(int size, std::string name, std::string* itemname, int* item);
	void multi_combobox(float indexes, std::string name, std::string* itemname, bool* item);
	void text(std::string name);

};