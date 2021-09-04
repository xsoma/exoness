#include "menu.h"

void old_ui::groupbox(int x, int y, int w, int h, std::string name, bool has_subtabs)
{
	auto text_size = render::get().get_text_size_rect(render::get().small_font, name.c_str()).left;

	setuped->rect_(Pos.x + x, Pos.y + y, w, h, Color(0, 0, 0, MenuAlpha_Main));

	setuped->rect_(Pos.x + x - 1, Pos.y + y - 1, w + 2, h + 2, Color(29, 29, 29, MenuAlpha_Main));

	if (!(name == ""))
		setuped->rect_fill_(Pos.x + x - 8 + (w / 2) - (text_size / 2), Pos.y + y - 7, text_size + 16, 13, Color(12, 12, 12, MenuAlpha_Main));

	setuped->text(Pos.x + x + (w / 2), Pos.y + y - 8, setuped->main_font, name.c_str(), Color(200, 200, 200, MenuAlpha_Main), true);

	groupbox_top = Pos.y + y;

	if (has_subtabs)
	{
		y_offset = (Pos.y + (y + 20));
	}
	else
	{
		y_offset = (Pos.y + (y + 12));
	}

	x_offset = Pos.x + x;

	groupbox_bottom = (Pos.y + (y + h));
	groupbox_width = (Pos.x + (x + w));

}