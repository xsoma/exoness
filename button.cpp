#include "menu.h"

void old_ui::button(ButtonCallback_t callback, std::string title)
{
	if (groupbox_bottom <= y_offset + 18)
		return;

	int ComboSize = 170;
	int ComboHeight = 30;

	static bool pressed = false;

	RECT text_size = setuped->text_size_to_rect(setuped->main_font, title.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset, ComboSize, ComboHeight) && menuOpened)
	{
		if (pressed)
		{
			if (callback)
				callback();
		}
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset, ComboSize, ComboHeight) && !pressed)
		pressed = true;


	setuped->vertical_gradient_2(x_offset + 10, y_offset, ComboSize, ComboHeight, Color(26, 26, 26), Color(18, 18, 18));
	
	setuped->rect_(x_offset + 10, y_offset, ComboSize, ComboHeight, Color(0, 0, 0, MenuAlpha_Main));

	if (render::get().mouse_in_region(x_offset + 10, y_offset, ComboSize, ComboHeight))
	{
		setuped->rect_(x_offset + 10, y_offset, ComboSize, ComboHeight, Color(112, 112, 112, MenuAlpha_Main));
	}

	setuped->text(x_offset + 10 + (ComboSize / 2) - (text_size.right / 2),
		y_offset + (30 / 2) - (text_size.bottom / 2) - 1, setuped->main_font, title.c_str(), Color(204, 204, 204, MenuAlpha_Main));

	y_offset += 36;
}