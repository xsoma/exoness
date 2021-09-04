#include "menu.h"

void old_ui::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 25)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;
	bool is_smth_checked = *item != 0;

	int ComboSize = 170; // 163

	int max_sizer = 19;

	static int offset = 0;
	if (name == "")
	{
		offset = -3;
	}
	else
	{
		offset = 14;
	}
	
	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset + offset, ComboSize, 20)
			&& !click_rest && !dont_recieve_input)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(
			x_offset + 10, y_offset + offset, ComboSize, 20))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}

	if (*item != 0)
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, name.c_str(), Color(200, 200, 200, MenuAlpha_Main));
	else
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, name.c_str(), Color(42, 42, 42, MenuAlpha_Main));

	setuped->vertical_gradient_2(x_offset + 10, y_offset + offset, ComboSize, max_sizer, Color(26, 26, 26), Color(18, 18, 18));
	
	setuped->rect_(x_offset + 10, y_offset + offset, ComboSize, max_sizer, Color(0, 0, 0, MenuAlpha_Main));
	
	if (open)
	{
		setuped->vertical_gradient_2(x_offset + 10, y_offset + offset + 2 + max_sizer, ComboSize, (size * max_sizer),
				Color(26, 26, 26), Color(20, 20, 20));
		
		setuped->rect_(x_offset + 10, y_offset + offset + 2 + max_sizer, ComboSize, (size * max_sizer),
			Color(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(
				x_offset + 10, y_offset + offset + 4 + 4 + 15 + i * max_sizer, ComboSize, max_sizer))
			{
				*item = i;
			}

			

			if (i == *item)
				setuped->text(x_offset + 10 + 5, y_offset + offset + 4 + 1 + 4 + 15 + i * max_sizer, setuped->main_font, itemname[i].c_str(), Color(200, 200, 200, MenuAlpha_Main));
			else
				setuped->text(x_offset + 10 + 5, y_offset + offset + 4 + 1 + 4 + 15 + i * max_sizer, setuped->main_font, itemname[i].c_str(), Color(65, 65, 65, MenuAlpha_Main));
		}
	}

	setuped->text(x_offset + 10 + 5, y_offset + offset + 3, setuped->main_font, itemname[*item].c_str(), is_smth_checked ? Color(200, 200, 200, MenuAlpha_Main) : Color(42, 42, 42, MenuAlpha_Main));

	// arrow(x_offset + 180, y_offset + 22, Color(153, 153, 153, MenuAlpha_Main));

	if (open)
	{
		if (name == "")
		{
			y_offset += 47 - 26 + (size * max_sizer);
		}
		else
		{
			y_offset += 47 + (size * max_sizer);
		}
	}
	else
	{
		if (name == "")
		{
			y_offset += 47 - 26;
		}
		else
		{
			y_offset += 41;
		}
	}
}