#pragma once
#include "direct_render.h"
#include "i_interfaces.h"

class d3d_menu : public singleton <d3d_menu>
{
public:

	struct
	{
		float x = 0.f, y = 0.f;
	}position;

	void gui_start();

	bool ui_open;

};