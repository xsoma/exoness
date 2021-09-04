#include "menu.h"
#include "vector_2d.h"

void gradient_v_end(int x, int y, int w, int h, Color c1, Color c2)
{
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (float i = 0; i < h / 1.5; i++)
	{
		float fi = i, fh = h / 1.5;
		float a = fi / fh;
		DWORD ia = a * 255;
		setuped->rect_fill_(x, y + i, w, 2, Color(first, second, third, ia));
	}
}

void GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	setuped->rect_fill_(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		setuped->rect_fill_(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void old_ui::color_selector(std::string name, int* red, int* green, int* blue, int* alpha)
{
	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163

	static int offset = 0;
	if (name == "sadasd")
	{
		offset = 0;
	}
	else
	{
		offset = 0;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 197 - 30, y_offset + 1, 12, 7) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 197 - 30, y_offset + 1, 12, 7))
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

	static unsigned int r = 255;
	static unsigned int g = 0;
	static unsigned int b = 0;

	int calculator = 470;
	int calc_y = 1;

	static unsigned int selected_r = 0;
	static unsigned int selected_g = 0;
	static unsigned int selected_b = 0;

	static Vector2D MousePos;
	COLORREF color;
	COLORREF selected_color;

	if (open)
	{
		dont_recieve_input = true;

		setuped->rect_fill_(Pos.x + 225 + calculator - ComboSize - 5, Pos.y, ComboSize, ComboSizeH - 10,
			Color(16, 16, 16, MenuAlpha_Main));

		setuped->rect_(Pos.x + 225 + calculator - ComboSize - 5 + 1, Pos.y + 1, ComboSize - 2, ComboSizeH - 2 - 10,
			Color(29, 29, 29, MenuAlpha_Main));

		float _pixel_value = 255 / (ComboSize - 5 - 17);

		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(Pos.x + 233 + calculator - ComboSize + 101 + 1,
			Pos.y - calc_y + 5 + 1, 7 - 2, 107 - 1))
		{
			HDC hdc = GetDC(nullptr);
			color = GetPixel(hdc, render::get().get_mouse_pos().x, render::get().get_mouse_pos().y);

			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
		}

		GradientH(Pos.x + 225 + calculator - ComboSize - 5 + 5, Pos.y - calc_y + 5, ComboSize - 5 - 17, ComboSizeH - 20,
			Color(255, 255, 255), Color(r, g, b));
		gradient_v_end(Pos.x + 225 + calculator - ComboSize - 5 + 5, Pos.y - calc_y + 40, ComboSize - 5 - 17, ComboSizeH - 20,
			Color(0, 0, 0), Color(0, 0, 0));

		setuped->rect_(Pos.x + 225 + calculator - ComboSize - 5 + 5, Pos.y - calc_y + 5, ComboSize - 5 - 15 - 1, ComboSizeH - 20 + 1,
			Color(29, 29, 29, MenuAlpha_Main));

		if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(Pos.x + 225 + calculator - ComboSize - 5 + 5 + 1,
			Pos.y - calc_y + 5 + 1, ComboSize - 5 - 17 - 2,
			ComboSizeH - 20 - 2))
		{
			HDC hdc = GetDC(nullptr);
			selected_color = GetPixel(hdc, render::get().get_mouse_pos().x, render::get().get_mouse_pos().y);

			selected_r = GetRValue(selected_color);
			selected_g = GetGValue(selected_color);
			selected_b = GetBValue(selected_color);

			*red = selected_r;
			*green = selected_g;
			*blue = selected_b;

			MousePos = Vector2D(render::get().get_mouse_pos().x, render::get().get_mouse_pos().y);
		}

		Color rainbow; int hue;

		for (int i = 0; i < 100; i++)
		{
			float hue = (i * .01f);
			rainbow.FromHSV(hue, 1.f, 1.f);

			setuped->rect_fill_(Pos.x + 233 + calculator - ComboSize + 101, Pos.y - calc_y + 5 + i, 7, 8, rainbow);
			setuped->rect_(Pos.x + 233 + calculator - ComboSize + 101, Pos.y - calc_y + 5, 7, 107, Color(29, 29, 29, 255));
		}
	}
	else
	{
		dont_recieve_input = false;
	}

	int to_set[3] = {
		*red - 20,
		*green - 20,
		*blue - 20
	};

	if (to_set[0] < 42) {
		to_set[0] = *red + 20;
	}

	if (to_set[1] < 42) {
		to_set[1] = *green + 20;
	}

	if (to_set[2] < 42) {
		to_set[2] = *blue + 20;
	}

	auto final_clr = Color(to_set[0], to_set[1], to_set[2]);

	if (menuOpened)
	{
		
		if (*red > 20 && *green > 20 && *blue > 20)
		{
			setuped->vertical_gradient_2(x_offset + 197 - 30, y_offset, 15, 10, Color(*red, *green, *blue, MenuAlpha_Main), final_clr);
		}
		else
		{
			setuped->vertical_gradient_2(x_offset + 197 - 30, y_offset, 15, 10, Color(*red, *green, *blue, MenuAlpha_Main), final_clr);
		}
		
	}

	setuped->rect_(x_offset + 197 - 30 - 1, y_offset - 1, 15 + 2, 12, Color(0, 0, 0, MenuAlpha_Main));
}