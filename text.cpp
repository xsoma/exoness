#include "menu.h"

void old_ui::text(std::string name)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	setuped->text(x_offset + 10, y_offset - 2, setuped->main_font, name.c_str(), Color(200, 200, 200, MenuAlpha_Text));

	y_offset += 17;
}