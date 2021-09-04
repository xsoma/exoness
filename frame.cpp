#include "frame.h"
#include "draw_manager.h"

void d3d_menu::gui_start()
{
	static Vector2D _mouse_pos;
	static int groupbox_bottom;
	static int dragX = 0;
	static int dragY = 0;
	static int Width = 750;
	static int Height = 550;
	static Vector2D oldPos;
	static Vector2D mousePos;
	static bool Dragging = false;
	bool click = false;
	static int iScreenWidth, iScreenHeight;
	static bool _pressed = true;

	if (!_pressed && GetAsyncKeyState(VK_INSERT))
		_pressed = true;
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		_pressed = false;
		ui_open = !ui_open;
	}

	interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
	interfaces::inputsys->EnableInput(!ui_open);

	if (ui_open)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
			click = true;

		Vector2D MousePos = render::get().get_mouse_pos();

		if (Dragging && !click)
		{
			Dragging = false;
		}

		if (Dragging && click)
		{
			position.x = MousePos.x - dragX;
			position.y = MousePos.y - dragY;
		}

		if (render::get().mouse_in_region(position.x, position.y, Width, 20))
		{
			Dragging = true;
			dragX = MousePos.x - position.x;
			dragY = MousePos.y - position.y;
		}

		if (position.x < 0)
			position.x = 0;
		if (position.y < 0)
			position.y = 0;
		if ((position.x + Width) > iScreenWidth)
			position.x = iScreenWidth - Width;
		if ((position.y + Height) > iScreenHeight)
			position.y = iScreenHeight - Height;

		setuped->rect_fill_(position.x, position.y, Width, Height, Color(35, 35, 35, 255));
		setuped->rect_(position.x, position.y, Width, Height, Color(58, 58, 58, 255));

		

	}

}