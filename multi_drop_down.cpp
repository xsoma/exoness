#include "menu.h"

void old_ui::multi_combobox(float indexes, std::string name, std::string* itemname, bool* item)
{
	if (groupbox_bottom <= y_offset + 25)
		return;

	static bool multiPressed = false;
	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	bool items = false;
	static std::string nameSelected;
	std::string itemsSelected = "";
	int lastItem = 0;
	int ComboSize = 170;

	int offset = 0;

	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset + offset, ComboSize, 17) && !clickRest
		)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset + offset, ComboSize, 20))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	setuped->vertical_gradient_2(x_offset + 10, y_offset + offset, ComboSize, 19, Color(26, 26, 26), Color(18, 18, 18));
	
	setuped->rect_(x_offset + 10, y_offset + offset, ComboSize, 19, Color(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		
		setuped->vertical_gradient_2(x_offset + 10, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			Color(26, 26, 26), Color(20, 20, 20));

		setuped->rect_(x_offset + 10, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			Color(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < indexes; i++)
		{
			if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(
				x_offset + 10, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 20))
			{
				if (multiPressed)
					item[i] = !item[i];
				multiPressed = false;
			}

			if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(
				x_offset + 10, y_offset + offset + 16 + (i * 19), ComboSize, 20) && !multiPressed)
				multiPressed = true;

	
			if (item[i])
				setuped->text(x_offset + 10 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, setuped->main_font, itemname[i].c_str(), Color(200, 200, 200, MenuAlpha_Main));
			else
				setuped->text(x_offset + 10 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, setuped->main_font, itemname[i].c_str(), Color(65, 65, 65, MenuAlpha_Main));
		}
	}

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{
			if (lastItem < i)
				lastItem = i;
		}
	}

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{

			items = true;
			RECT TextSize = setuped->text_size_to_rect(setuped->main_font, itemsSelected.c_str());
			RECT TextSizeGonaAdd = setuped->text_size_to_rect(setuped->main_font, itemname[i].c_str());
			if (TextSize.right + TextSizeGonaAdd.right + 30 < ComboSize)
				itemsSelected += itemname[i] + ((lastItem == i) ? "" : ", ");
		}
	}

	if (!items)
		itemsSelected = "none";

	
	if (items)
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, name.c_str(), Color(200, 200, 200, MenuAlpha_Main));
	else
		setuped->text(x_offset + 10, y_offset - 3, setuped->main_font, name.c_str(), Color(42, 42, 42, MenuAlpha_Main));

	if (items)
		setuped->text(x_offset + 10 + 5, y_offset + offset + 3, setuped->main_font, itemsSelected.c_str(), Color(200, 200, 200, MenuAlpha_Main));
	else
		setuped->text(x_offset + 10 + 5, y_offset + offset + 3, setuped->main_font, itemsSelected.c_str(), Color(42,42, 42, MenuAlpha_Main));

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
		y_offset += 52 + (indexes * 16);
	else
		y_offset += 41;
}