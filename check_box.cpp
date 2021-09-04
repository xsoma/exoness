#include "menu.h"

void old_ui::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	int size = 10;

	static bool pressed = false;

	RECT text_size = setuped->text_size_to_rect(setuped->main_font, name.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 170, y_offset, size,
		size) && !dont_recieve_input && menuOpened)
	{
		if (pressed)
			*item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 170, y_offset, size,
		size) && !pressed)
		pressed = true;

	if (*item == true)
	{
		setuped->vertical_gradient_2(x_offset + 170, y_offset, size, size, Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b, MenuAlpha_Main), c_config::get().gradient_menu_col);
		
		setuped->text(x_offset + 10, y_offset - 2, setuped->main_font, name.c_str(), Color(200, 200, 200, MenuAlpha_Text));

	}
	else
	{

		setuped->text(x_offset + 10, y_offset - 2, setuped->main_font, name.c_str(), Color(42, 42, 42, MenuAlpha_Main));

		setuped->vertical_gradient_2(x_offset + 170, y_offset, size, size, Color(23, 23, 23), Color(18, 18, 18));
	}

	if (render::get().mouse_in_region(x_offset + 170, y_offset, size, size))
		setuped->rect_(x_offset + 170, y_offset, size, size, Color(180, 180, 180, MenuAlpha_Main));
	else
		setuped->rect_(x_offset + 170, y_offset, size, size, Color(0, 0, 0, MenuAlpha_Main));

	y_offset += 17;
}