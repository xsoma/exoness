#include "menu.h"

void old_ui::slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 30)
		return;

	float SliderSize = 170; // 163

	float _pixel_value = max / SliderSize;
	static bool opened = false;

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10 - 1, y_offset + 14,
			SliderSize + (3 * 2), 7) && !dont_recieve_input && menuOpened)
		{
			*item = abs(render::get().get_mouse_pos().x - (x_offset + 10)) * _pixel_value;
		}

		static bool pressed_subtract = false;
		static bool pressed_add = false;
	}

	std::string namedisplay;

	namedisplay += name.c_str();

	if (*item > max)
	{
		*item = max;
	}

	if (*item < 0)
	{
		*item = 0;
	}

	

	if (*item > 0)
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, namedisplay.c_str(), Color(200, 200, 200, MenuAlpha_Text));
	else
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, namedisplay.c_str(), Color(42, 42, 42, MenuAlpha_Text));

	setuped->vertical_gradient_2(x_offset + 10, y_offset + 14, SliderSize, 7, Color(23, 23, 23), Color(18, 18, 18));
	
	UINT hp = SliderSize - *item / max;

	float az = *item / max;

	int Red = 255 - ((SliderSize * (*item / _pixel_value) * 2.55));
	int Green = (SliderSize * (*item / _pixel_value) * 2.55);

	if (*item)
	{
		if (menuOpened)
		{
			
			if (*item == max)
				setuped->vertical_gradient_2(x_offset + 10, y_offset + 14, SliderSize, 7,
					Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b,
						MenuAlpha_Main), c_config::get().gradient_menu_col);
			else
				setuped->vertical_gradient_2(x_offset + 10, y_offset + 14, (*item / _pixel_value), 7,
					Color(c_config::get().menu_color_r, c_config::get().menu_color_g, c_config::get().menu_color_b,
						MenuAlpha_Main), c_config::get().gradient_menu_col);
			
		}
	}

	setuped->rect_(x_offset + 10, y_offset + 14, SliderSize, 7, Color(0, 0, 0, MenuAlpha_Main));

	std::string gay2;
	gay2 += std::to_string(*item);
	gay2 += suffix.c_str();

	auto ratio = *item / max;
	auto location = SliderSize * ratio;

	if (*item > 0)
		setuped->text((*item / _pixel_value) + 10 + x_offset, y_offset + 15, setuped->visualz_font, std::to_string(*item).c_str(), Color(200, 200, 200, MenuAlpha_Text), false, 2);

	y_offset += 30;
}