#include "menu.h"

bool manually_set_key = false;

bool keys[256];
bool oldKeys[256];

bool GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	return false;
}

const char* KeyStringsStick[254] = {
	"INVLD", "M1", "M2", "BRK", "M3", "M4", "M5",
	"INVLD", "BSPC", "TAB", "INVLD", "INVLD", "INVLD", "ENTER", "INVLD", "INVLD", "SHI",
	"CTRL", "ALT", "PAU", "CAPS", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"ESC", "INVLD", "INVLD", "INVLD", "INVLD", "SPACE", "PGUP", "PGDOWN", "END", "HOME", "LEFT",
	"UP", "RIGHT", "DOWN", "INVLD", "PRNT", "INVLD", "PRTSCR", "INS", "DEL", "INVLD", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X", "Y", "Z", "LFTWIN", "RGHTWIN", "INVLD", "INVLD", "INVLD", "NUM0", "NUM1",
	"NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
	"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",
	"F21",
	"F22", "F23", "F24", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"NUM LOCK", "SCROLL LOCK", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "LSHFT", "RSHFT", "LCTRL",
	"RCTRL", "LMENU", "RMENU", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "NTRK", "PTRK", "STOP", "PLAY", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", ";", "+", ",", "-", ".", "/?", "~", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "{", "\\|", "}", "'\"", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD"
};

void old_ui::keybind(int* item, std::string name)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	int ComboSize = 170; // 163
	static std::string nameSelected;


	static bool IsGettingKey = false;
	static int Key = 0;
	static bool GoodKeyName = false;
	Color text_color = Color(200, 200, 200, MenuAlpha_Main);
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	char NameBuffer[128];
	const char* KeyName = "unbinded";
	RECT text_size = setuped->text_size_to_rect(setuped->main_font, KeyName);
	static bool LetsBindy0 = false;

	if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(x_offset + 10, y_offset, 157, 20) && menuOpened)
	{
		if (!IsGettingKey)
		{
			IsGettingKey = true;
		}
	}

	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Key = -1;
					*item = Key;
					IsGettingKey = false;
					return;
				}

				Key = i;
				*item = Key;
				IsGettingKey = false;
				return;
			}
		}
	}


	if (IsGettingKey)
	{
		KeyName = "-";
		text_color = Color(200, 200, 200, MenuAlpha_Main);
	}
	else
	{
		if (*item >= 0)
		{
			KeyName = KeyStringsStick[*item];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(*item << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "unbinded";
		}
	}

	if (manually_set_key)
	{
		Key = *item;
		manually_set_key = false;
	}

	
	setuped->vertical_gradient_2(x_offset + 10, y_offset, ComboSize, 20, Color(26, 26, 26), Color(20, 20, 20));

	setuped->rect_(x_offset + 10, y_offset, ComboSize, 20, Color(0, 0, 0, MenuAlpha_Main));

	
	if (KeyName == "unbinded")
		setuped->text(x_offset + 10 + 5, y_offset + 3, setuped->main_font, KeyName, Color(42, 42, 42, MenuAlpha_Main));

	else
		setuped->text(x_offset + 10 + 5, y_offset + 3, setuped->main_font, KeyName, Color(200, 200, 200, MenuAlpha_Main));


	y_offset += 30;
}