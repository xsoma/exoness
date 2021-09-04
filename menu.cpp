#include "menu.h"
#include "i_interfaces.h"
#include "setup.h"

#include "picture.h"

static int save_slot = 0;
static int bind_slot = 0;

std::string pitch_make[4] = { "none", "down", "up", "zero" };
std::string model_mode[5] = { "none", "enemy", "teammate", "local", "fake yaw"};
std::string chams_enemy[5] = { "none", "default", "flat", "glow", "eso glow" };
std::string chams_team[5] = { "none", "default", "flat", "glow", "eso glow" };
std::string chams_local[5] = { "none", "default", "flat", "glow", "eso glow" };
std::string chams_fake[5] = { "none", "default", "flat", "glow", "eso glow" };
std::string config_slots[7] = { "none", "1", "2", "3", "4", "5", "6" };
std::string removals_[5] = { "scope", "flash", "smoke", "visual recoil", "post processing" };
std::string flags_esp[3] = { "scoped", "armor" };
std::string world[3] = { "none", "context", "light" };
std::string strafe_type[3] = { "none", "default", "viewangles" };
std::string binds_[5] = { "none", "thirdperson", "inverter", "exploit", "slow-walk" };
std::string yaw_[2] = { "none", "180" };
std::string hitbox_priority[3] = { "none", "head", "body" };
std::string weapons[7] = { "none", "auto", "scout", "awp", "pistol", "heavy pistol", "other" };
std::string pitch_[4] = { "none", "down", "zero", "up" };
std::string target_selection[4] = { "none", "fov", "health", "distance" };
std::string fake_yaw_[4] = { "none", "static", "sway", "opposite" };
std::string fakelag_mode[3] = {	"none", "factor", "adaptive" };
std::string exploit_active[3] = { "none", "double tap" };

enum cfg_mode
{
	AUTO_,
	SCOUT_,
	AWP_,
	PISTOL_,
	HEAVY_PISTOL_,
	OTHER_
};

void save_config()
{
	if (save_slot != 0)
		csgo_config->Save(config_slots[save_slot]);
}

void load_config()
{
	if (save_slot != 0)
		csgo_config->Load(config_slots[save_slot]);
}

LPDIRECT3DTEXTURE9 img;
LPD3DXSPRITE sprite;

void old_ui::on_render(IDirect3DDevice9* pDevice)
{
	static Vector2D _mouse_pos;
	static int groupbox_bottom;
	static int dragX = 0;
	static int dragY = 0;
	static int Width = 550;
	static int Height = 350;
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
		menuOpened = !menuOpened;
	}

	interfaces::engine->GetScreenSize(screen_width, screen_width);
	interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
	interfaces::inputsys->EnableInput(!menuOpened);

	if (menuOpened)
	{
		MenuAlpha_Main = 255;
		MenuAlpha_Text = 255;

		if (GetAsyncKeyState(VK_LBUTTON))
			click = true;

		Vector2D MousePos = render::get().get_mouse_pos();

		if (Dragging && !click)
		{
			Dragging = false;
		}

		if (Dragging && click)
		{
			Pos.x = MousePos.x - dragX;
			Pos.y = MousePos.y - dragY;
		}

		if (render::get().mouse_in_region(Pos.x, Pos.y, Width, 20))
		{
			Dragging = true;
			dragX = MousePos.x - Pos.x;
			dragY = MousePos.y - Pos.y;
		}

		if (Pos.x < 0)
			Pos.x = 0;
		if (Pos.y < 0)
			Pos.y = 0;
		if ((Pos.x + Width) > iScreenWidth)
			Pos.x = iScreenWidth - Width;
		if ((Pos.y + Height) > iScreenHeight)
			Pos.y = iScreenHeight - Height;

		_pos.x = Pos.x + 85;
		_pos.y = Pos.y + 25;
	}
	else
	{
		MenuAlpha_Main = 0;
		MenuAlpha_Text = 0;
	}

	if (menuOpened) {

		int value = setuped->text_size_to_rect(setuped->main_font, "exoness [tweex-cs:go]").right;

		static int aimbot_main_subtab = 0;
		static int visuals_main_subtab = 0;
		static int misc_main_subtab = 0;
		static int tab_selected = 0;

		bool is_eso = c_config::get().chams_mode_enemy == 3 || c_config::get().chams_mode_enemy == 4;
		bool is_eso_team = c_config::get().chams_mode_teammate == 3 || c_config::get().chams_mode_teammate == 4;
		bool is_eso_local = c_config::get().chams_mode_local == 3 || c_config::get().chams_mode_local == 4;
		bool is_eso_fake = c_config::get().chams_mode_fake == 3 || c_config::get().chams_mode_fake == 4;

		int gradient[3] = {
			c_config::get().menu_color_r - 20,
			c_config::get().menu_color_g - 20,
			c_config::get().menu_color_b - 20
		};

		if (gradient[0] < 42) {
			gradient[0] = c_config::get().menu_color_r + 20;
		}

		if (gradient[1] < 42) {
			gradient[1] = c_config::get().menu_color_g + 20;
		}

		if (gradient[2] < 42) {
			gradient[2] = c_config::get().menu_color_b + 20;
		}

		c_config::get().gradient_menu_col = Color(gradient[0], gradient[1], gradient[2]);

		setuped->rect_fill_(Pos.x, Pos.y, Width, Height, Color(12, 12, 12, MenuAlpha_Main));

		setuped->rect_(Pos.x + 1, Pos.y + 1, Width - 2, Height - 2, Color(29, 29, 29, MenuAlpha_Main));
		setuped->rect_(Pos.x + 1, Pos.y + 1, Width - 2, 24, Color(29, 29, 29, MenuAlpha_Main));

		setuped->rect_fill_(Pos.x + 1, Pos.y + 26, Width - 2, 1, Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main - 200));
		setuped->rect_fill_(Pos.x + 1, Pos.y + 27, Width - 2, 1, Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main));

		setuped->rect_(Pos.x + 8, Pos.y + 36, 120, 185, Color(29, 29, 29, MenuAlpha_Main));
		setuped->rect_(Pos.x + 8 + 1, Pos.y + 36 + 1, 120 - 2, 185 - 2, Color(0, 0, 0, MenuAlpha_Main));

		setuped->rect_(Pos.x + 8, Pos.y + 228, 120, 114, Color(29, 29, 29, MenuAlpha_Main));
		setuped->rect_(Pos.x + 8 + 1, Pos.y + 228 + 1, 120 - 2, 114 - 2, Color(0, 0, 0, MenuAlpha_Main));

		setuped->vertical_gradient(Pos.x + 10, Pos.y + 228 + 7, 1, 114 - 7, Color(16, 16, 16, MenuAlpha_Main), Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main - 100));
		setuped->vertical_gradient(Pos.x + 125, Pos.y + 228 + 7, 1, 114 - 7, Color(16, 16, 16, MenuAlpha_Main), Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main - 100));

		setuped->text(Pos.x + 6, Pos.y + 6, setuped->main_font, "exoness [tweex-cs:go]", Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main));
		
		setuped->text(Pos.x + 21, Pos.y + 242, setuped->gang, "TW", Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main));
		setuped->text(Pos.x + 36, Pos.y + 315, setuped->pixel_font, "t w e e x cs:go", Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main));


		int in_sizew = Width - 50;

		static const int tab_amount = 5;
		static std::string tab_names[tab_amount] = { "aimbot", "visuals", "misc", "skins", "config" };
		
		for (int i = 0; i < tab_amount; i++)
		{
			RECT text_size = setuped->text_size_to_rect(setuped->main_font, tab_names[i].c_str());

			int tab_area[4] = { Pos.x + 25, Pos.y + 47 + (i * ((in_sizew / tab_amount) / 5) - 5), (in_sizew / tab_amount), 17 };

			if (GetAsyncKeyState(VK_LBUTTON) && render::get().mouse_in_region(tab_area[0], tab_area[1], tab_area[2],
				tab_area[3]))
				tab_selected = i;
			
			if (tab_selected == i)
			{
				setuped->gradient_h_(tab_area[0] - 9, tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), 100, 17, Color(35, 35, 35, 180), Color(14, 14, 14, 140));

				setuped->rect_fill_(tab_area[0] - 9, tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), 1, 17, Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main));

				setuped->text(tab_area[0] - 9 + 7,
					tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2) + 1, setuped->main_font, tab_names[i].c_str(), Color(200, 200, 200, MenuAlpha_Main));
			}
			else
			{
				setuped->gradient_h_(tab_area[0] - 9, tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), 100, 17, Color(24, 24, 24, 140), Color(16, 16, 16, 140));

				setuped->rect_fill_(tab_area[0] - 9, tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), 1, 17, Color(c_config::get().c_config::get().menu_color_r, c_config::get().c_config::get().menu_color_g, c_config::get().c_config::get().menu_color_b, MenuAlpha_Main - 250));

				setuped->text(tab_area[0] - 9 + 7,
					tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2) + 1, setuped->main_font, tab_names[i].c_str(), Color(42, 42, 42, MenuAlpha_Main));
			}
		}

		
		if (tab_selected == 0)
		{
			
			this->draw_aimbot_tab(aimbot_main_subtab, 4, _pos, MenuAlpha_Main);

			if (aimbot_main_subtab == 0)
			{
				this->groupbox(145, 70, 190, 265, "primary", true);
				{	
					this->checkbox("enabled", &c_config::get().aimbot_enabled);
					this->slider(360, "maximum fov", &c_config::get().max_fov, "", 0);
					this->combobox(4, "target selection", target_selection, &c_config::get().target_selection);
					
				}

				this->groupbox(344, 70, 190, 265, "other", true);
				{				
					static int value_main = 0;
					static int weapon_mode = 0;

					switch (weapon_mode)
					{
					case 1:
						value_main = cfg_mode::AUTO_;
						break;
					case 2:
						value_main = cfg_mode::SCOUT_;
						break;
					case 3:
						value_main = cfg_mode::AWP_;
						break;
					case 4:
						value_main = cfg_mode::PISTOL_;
						break;
					case 5:
						value_main = cfg_mode::HEAVY_PISTOL_;
						break;
					case 6:
						value_main = cfg_mode::OTHER_;
						break;
					}

					this->combobox(7, "weapon setting", weapons, &weapon_mode);

					if (weapon_mode > 0)
					{
						this->slider(100, "minimum damage", &c_config::get().damage[value_main], "", 0);
						this->slider(100, "minimum hitchance", &c_config::get().hitchance[value_main], "", 0);
						this->slider(100, "head scale", &c_config::get().head_scale[value_main], "", 0);
						this->slider(100, "body scale", &c_config::get().body_scale[value_main], "", 0);
						this->combobox(3, "priority hitbox", hitbox_priority, &c_config::get().priority[value_main]);
					}
					
				}
			}
			else if (aimbot_main_subtab == 1)
			{
				this->groupbox(145, 70, 190, 265, "bodyaim conditions", true);
				{
					this->checkbox("lethal", &c_config::get().baim_lethal);
					this->checkbox("in air", &c_config::get().baim_air);
					this->checkbox("force accuracy", &c_config::get().force);
				}
			}
			else if (aimbot_main_subtab == 2)
			{
				this->groupbox(145, 70, 190, 265, "real", true);
				{
					this->combobox(2, "yaw", yaw_, &c_config::get().yaw);
					this->combobox(4, "pitch", pitch_, &c_config::get().pitch);
					this->slider(90, "add jitter", &c_config::get().jitter, "", 0);

					if (c_config::get().invert_real)
					{
						this->slider(100, "body lean", &c_config::get().body_lean, "", 0);
						this->slider(100, "inverted body lean", &c_config::get().inverted_body_lean, "", 0);
					}

					this->checkbox("invert", &c_config::get().invert_real);
					this->checkbox("fix leg movement", &c_config::get().leg);
					
				}

				this->groupbox(344, 70, 190, 265, "fake", true);
				{
					
					this->combobox(4, "fake yaw", fake_yaw_, &c_config::get().fake_mode);

					this->slider(100, "left fake amount", &c_config::get().fake_left, "", 0);
					this->slider(100, "right fake amount", &c_config::get().fake_right, "", 0);

					this->checkbox("invert ", &c_config::get().invert_fake);

				}
			}
			else if (aimbot_main_subtab == 3)
			{
				this->groupbox(145, 70, 190, 265, "fakelag", true);
				{
					this->checkbox("enabled", &c_config::get().fake_lag_start);
					this->combobox(3, "mode", fakelag_mode, &c_config::get().fakelag_mode);
					this->slider(14, "limit", &c_config::get().lag_limit, "", 1);
				}

				this->groupbox(344, 70, 190, 265, "exploits", true);
				{
					this->combobox(2, "active", exploit_active, &c_config::get().active_exploit);
				}
			}
		}
		else if (tab_selected == 1)
		{
			this->draw_visuals_tab(visuals_main_subtab, 4, _pos, MenuAlpha_Main);

			if (visuals_main_subtab == 0)
			{
				this->groupbox(145, 70, 190, 265, "players", true);
				{
					this->checkbox("enabled", &c_config::get().visuals_enabled);

					this->color_selector("clr_box", &c_config::get().box_r, &c_config::get().box_g, &c_config::get().box_b, &c_config::get().box_a);
					this->checkbox_with_picker("box", &c_config::get().draw_box);

					this->color_selector("clr_name", &c_config::get().name_r, &c_config::get().name_g, &c_config::get().name_b, &c_config::get().name_a);
					this->checkbox_with_picker("name", &c_config::get().draw_name);

					this->color_selector("clr_hp", &c_config::get().hp_r, &c_config::get().hp_g, &c_config::get().hp_b, &c_config::get().hp_a);
					this->checkbox_with_picker("health bar", &c_config::get().draw_health);

					this->color_selector("clr_ammo", &c_config::get().ammo_r, &c_config::get().ammo_g, &c_config::get().ammo_b, &c_config::get().ammo_a);
					this->checkbox_with_picker("ammo bar", &c_config::get().draw_ammo);

					this->color_selector("clr_weapn", &c_config::get().weapon_r, &c_config::get().weapon_g, &c_config::get().weapon_b, &c_config::get().weapon_a);
					this->checkbox_with_picker("weapon", &c_config::get().draw_weapon);

					this->color_selector("clr_glow", &c_config::get().glow_esp_color_r, &c_config::get().glow_esp_color_g, &c_config::get().glow_esp_color_b, &c_config::get().glow_esp_color_a);
					this->checkbox_with_picker("glow", &c_config::get().glow_enemies);

					this->color_selector("clr_oof", &c_config::get().fov_arrows_esp_color_r, &c_config::get().fov_arrows_esp_color_g, &c_config::get().fov_arrows_esp_color_b, &c_config::get().fov_arrows_esp_color_a);
					this->checkbox_with_picker("out of fov arrows", &c_config::get().fov_arrows_enemy);

					if (c_config::get().fov_arrows_enemy)
					{
						this->slider(30, "size", &c_config::get().fov_arrows_enemy_size, "", 1);
						this->slider(100, "distance", &c_config::get().fov_arrows_enemy_distance, "", 1);
					}

					this->multi_combobox(2, "flags", flags_esp, c_config::get().flags);
				}

				this->groupbox(344, 70, 190, 265, "weapons", true);
				{
					this->checkbox("enabled ", &c_config::get().visuals_world_enabled);

					this->checkbox("radius", &c_config::get().radius);

					this->color_selector("color_box_weapon", &c_config::get().weapon_box_r, &c_config::get().weapon_box_g, &c_config::get().weapon_box_b, &c_config::get().weapon_box_a);
					this->checkbox_with_picker("box ", &c_config::get().weapon_draw_box);

					this->color_selector("color_box_name", &c_config::get().weapon_name_r, &c_config::get().weapon_name_g, &c_config::get().weapon_name_b, &c_config::get().weapon_name_a);
					this->checkbox_with_picker("name ", &c_config::get().weapon_draw_name);

					this->color_selector("color_box_ammo", &c_config::get().weapon_ammo_r, &c_config::get().weapon_ammo_g, &c_config::get().weapon_ammo_b, &c_config::get().weapon_ammo_a);
					this->checkbox_with_picker("ammo ", &c_config::get().weapon_draw_ammo);

					this->color_selector("color_box_glow", &c_config::get().glow_weapon_r, &c_config::get().glow_weapon_g, &c_config::get().glow_weapon_b, &c_config::get().glow_weapon_a);
					this->checkbox_with_picker("glow ", &c_config::get().weapon_draw_glow);
				}
			}
			else if (visuals_main_subtab == 1)
			{
				this->groupbox(145, 70, 190, 265, "model", true);
				{
					this->combobox(5, "models", model_mode, &c_config::get().model_mode);
					
					if (c_config::get().chams_mode_enemy > 0 && c_config::get().model_mode == 1)
					{
						this->slider(255, "transparency", &c_config::get().transparency, "", 1);

						if (is_eso && c_config::get().gloss_effect)
							this->slider(255, "second transparency", &c_config::get().transparency_second, "", 1);
					}
					else if (c_config::get().chams_mode_teammate > 0 && c_config::get().model_mode == 2)
					{
						this->slider(255, "transparency", &c_config::get().transparency_team, "", 1);

						if (is_eso_team && c_config::get().gloss_effect_team)
							this->slider(255, "second transparency", &c_config::get().transparency_second_team, "", 1);
					}
					else if (c_config::get().chams_mode_local > 0 && c_config::get().model_mode == 3)
					{
						this->slider(255, "transparency", &c_config::get().transparency_local, "", 1);

						if (is_eso_local && c_config::get().gloss_local)
							this->slider(255, "second transparency", &c_config::get().transparency_second_local, "", 1);
					}
					else if (c_config::get().chams_mode_fake > 0 && c_config::get().model_mode == 4)
					{
						this->slider(255, "transparency", &c_config::get().transparency_fake, "", 1);

						if (is_eso_fake && c_config::get().gloss_fake)
							this->slider(255, "second transparency", &c_config::get().transparency_second_fake, "", 1);

						this->checkbox("interpolate", &c_config::get().interpolate);
					}
				}

				this->groupbox(344, 70, 190, 265, "setting", true);
				{
					if (c_config::get().model_mode == 1)
					{
						this->combobox(5, "material", chams_enemy, &c_config::get().chams_mode_enemy);

						if (c_config::get().chams_mode_enemy > 0)
						{
							this->checkbox("wireframe", &c_config::get().wireframe);

							this->color_selector("clr_visible", &c_config::get().chams_esp_color_r, &c_config::get().chams_esp_color_g, &c_config::get().chams_esp_color_b, &c_config::get().chams_esp_color_a);
							this->checkbox_with_picker("visible ", &c_config::get().chams_enemies);

							if (c_config::get().chams_enemies)
							{
								this->color_selector("clr_xqz", &c_config::get().chams_xqz_esp_color_r, &c_config::get().chams_xqz_esp_color_g, &c_config::get().chams_xqz_esp_color_b, &c_config::get().chams_xqz_esp_color_a);
								this->checkbox_with_picker("invisible ", &c_config::get().chams_xqz_enemies);
							}

							if (is_eso)
							{
								this->color_selector("clr_gloss", &c_config::get().gloss_effect_r, &c_config::get().gloss_effect_g, &c_config::get().gloss_effect_b, &c_config::get().gloss_effect_a);
								this->checkbox_with_picker("second layer", &c_config::get().gloss_effect);

								if (c_config::get().chams_xqz_enemies)
								{
									this->color_selector("clr_gloss_xqz", &c_config::get().gloss_effect_r_xqz, &c_config::get().gloss_effect_g_xqz, &c_config::get().gloss_effect_b_xqz, &c_config::get().gloss_effect_a_xqz);
									this->checkbox_with_picker("second layer xqz", &c_config::get().gloss_effect_xqz);
								}

								this->checkbox("second wireframe", &c_config::get().wireframe_second);
							}
						}
					}
					else if (c_config::get().model_mode == 2)
					{
						this->combobox(5, "material ", chams_team, &c_config::get().chams_mode_teammate);

						if (c_config::get().chams_mode_teammate > 0)
						{
							this->checkbox("wireframe ", &c_config::get().wireframe_team);

							this->color_selector("clr_visible_t ", &c_config::get().chams_esp_color_r_team, &c_config::get().chams_esp_color_g_team, &c_config::get().chams_esp_color_b_team, &c_config::get().chams_esp_color_a_team);
							this->checkbox_with_picker("visible ", &c_config::get().chams_team);

							if (c_config::get().chams_team)
							{
								this->color_selector("clr_xqz_t ", &c_config::get().chams_xqz_esp_color_r_team, &c_config::get().chams_xqz_esp_color_g_team, &c_config::get().chams_xqz_esp_color_b_team, &c_config::get().chams_xqz_esp_color_a_team);
								this->checkbox_with_picker("invisible ", &c_config::get().chams_xqz_team);
							}

							if (is_eso_team)
							{
								this->color_selector("clr_gloss_t", &c_config::get().gloss_effect_r_team, &c_config::get().gloss_effect_g_team, &c_config::get().gloss_effect_b_team, &c_config::get().gloss_effect_a_team);
								this->checkbox_with_picker("second layer ", &c_config::get().gloss_effect_team);

								if (c_config::get().chams_xqz_team)
								{
									this->color_selector("clr_gloss_xqz_t ", &c_config::get().gloss_effect_r_xqz_team, &c_config::get().gloss_effect_g_xqz_team, &c_config::get().gloss_effect_b_xqz_team, &c_config::get().gloss_effect_a_xqz_team);
									this->checkbox_with_picker("second layer xqz ", &c_config::get().gloss_effect_xqz_team);
								}

								this->checkbox("second wireframe ", &c_config::get().wireframe_second_team);
							}
						}
					}
					else if (c_config::get().model_mode == 3)
					{
						this->combobox(5, "material  ", chams_local, &c_config::get().chams_mode_local);

						if (c_config::get().chams_mode_local > 0)
						{
							this->checkbox("wireframe  ", &c_config::get().wireframe_local);
							this->color_selector("clr_local_chams", &c_config::get().chams_color_local_r, &c_config::get().chams_color_local_g, &c_config::get().chams_color_local_b, 0);
							this->checkbox_with_picker("visible  ", &c_config::get().chams_local);

							if (is_eso_local)
							{
								this->color_selector("clr_gloss_local", &c_config::get().gloss_color_local_r, &c_config::get().gloss_color_local_g, &c_config::get().gloss_color_local_b, 0);
								this->checkbox_with_picker("second layer  ", &c_config::get().gloss_local);
								this->checkbox("second wireframe  ", &c_config::get().second_wireframe_local);
							}
						}
					}
					else if (c_config::get().model_mode == 4)
					{
						this->combobox(5, "material  ", chams_fake, &c_config::get().chams_mode_fake);

						if (c_config::get().chams_mode_fake > 0)
						{
							this->checkbox("wireframe  ", &c_config::get().wireframe_fake);
							this->color_selector("clr_local_fake", &c_config::get().chams_color_fake_r, &c_config::get().chams_color_fake_g, &c_config::get().chams_color_fake_b, 0);
							this->checkbox_with_picker("visible  ", &c_config::get().chams_fake);

							if (is_eso_fake)
							{
								this->color_selector("clr_gloss_fake", &c_config::get().gloss_color_fake_r, &c_config::get().gloss_color_fake_g, &c_config::get().gloss_color_fake_b, 0);
								this->checkbox_with_picker("second layer  ", &c_config::get().gloss_fake);
								this->checkbox("second wireframe  ", &c_config::get().second_wireframe_fake);
							}
						}
					}
				}
			}
			else if (visuals_main_subtab == 2)
			{
				this->groupbox(145, 70, 190, 265, "various", true);
				{
					this->checkbox("remove zoom", &c_config::get().remove_zoom);
					this->slider(180, "field of view", &c_config::get().fov, "", 0);
					this->slider(135, "thirdperson distance", &c_config::get().thirdperson_distance, "", 0);
					this->multi_combobox(5, "removals", removals_, c_config::get().remove);
				}

				this->groupbox(344, 70, 190, 265, "world", true);
				{
					this->combobox(3, "filter", world, &c_config::get().change_world);

					if (c_config::get().change_world == 1)
					{
						this->checkbox("modulate world", &c_config::get().world_change);

						if (c_config::get().world_change)
						{
							this->color_selector("clr_wall", &c_config::get().wall_color_r, &c_config::get().wall_color_g, &c_config::get().wall_color_b, &c_config::get().wall_color_a);
							this->checkbox_with_picker("wall", &c_config::get().wall);

							this->color_selector("clr_sky", &c_config::get().sky_color_r, &c_config::get().sky_color_g, &c_config::get().sky_color_b, &c_config::get().sky_color_a);
							this->checkbox_with_picker("sky", &c_config::get().sky);
							
						}
					}
					else if (c_config::get().change_world == 2)
					{
						this->checkbox("modulate light", &c_config::get().light_change);

						if (c_config::get().light_change)
						{
							this->slider_float(2.0, "exposure", &c_config::get().exposure, "", 0);
							this->slider_float(10.0, "bloom", &c_config::get().bloom, "", 0);
							this->slider_float(10.0, "ambient", &c_config::get().ambient, "", 0);
						}
					}
				}
			}
		}
		else if (tab_selected == 2)
		{
			this->draw_misc_tab(misc_main_subtab, 2, _pos, MenuAlpha_Main);

			if (misc_main_subtab == 0)
			{
				this->groupbox(145, 70, 190, 265, "movement", true);
				{
					this->checkbox("bhop", &c_config::get().bhop);
					this->slider(100, "slow-walk", &c_config::get().slow_walk, "", 0);
					this->combobox(3, "auto strafe", strafe_type, &c_config::get().strafe_type);
				}

				this->groupbox(344, 70, 190, 265, "game", true);
				{
					this->checkbox("hitsound", &c_config::get().hitsound);
				}
				
			}
			else if (misc_main_subtab == 1)
			{
				this->groupbox(145, 70, 190, 265, "binds", true);
				{
					this->combobox(5, "bind", binds_, &bind_slot);

					if (bind_slot == 1)
						this->keybind(&c_config::get().tp_key, "");
					else if (bind_slot == 2)
						this->keybind(&c_config::get().invert_key, "");
					else if (bind_slot == 3)
						this->keybind(&c_config::get().dt_key, "");
					else if (bind_slot == 4)
						this->keybind(&c_config::get().slow_key, "");

					if (bind_slot == 1)
						this->checkbox("hold", &c_config::get().hold_tp);
					else if (bind_slot == 2)
						this->checkbox("hold", &c_config::get().hold_invert);
					else if (bind_slot == 3)
						this->checkbox("hold", &c_config::get().hold_dt);
				}
			}

		}	
		else if (tab_selected == 4)
		{
			this->groupbox(145, 46, 190, 287, "menu", false);
			{
				this->color_selector("clr_watermark", &c_config::get().menu_color_r, &c_config::get().menu_color_g, &c_config::get().menu_color_b, &c_config::get().menu_color_a);
				this->text("menu theme");			
			}

			this->groupbox(344, 46, 190, 287, "config", false);
			{
				this->combobox(7, "configuration", config_slots, &save_slot);

				if (save_slot != 0)
				{
					this->button(save_config, "save");
					this->button(load_config, "load");
				}
			}
		}
		

		setuped->rect_(Pos.x + 138, Pos.y + 36, 403, 306, Color(29, 29, 29, MenuAlpha_Main));
	}
}