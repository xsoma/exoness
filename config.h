#pragma once
#include "singleton.h"
#include "color.h"

class c_config : public singleton< c_config > {
private:

public:

	bool remove[6] = { false, false, false, false, false };
	bool flags[3] = { false, false, false };
	bool chams_enemies = false;
	bool remove_zoom = false;
	int fov = 0;
	bool chams_xqz_enemies = false;
	int menu_color_r = 255, menu_color_g = 255, menu_color_b = 255, menu_color_a = 255;
	int wall_color_r = 255, wall_color_g = 255, wall_color_b = 255, wall_color_a = 255;
	int sky_color_r = 255, sky_color_g = 255, sky_color_b = 255, sky_color_a = 255;
	bool sky = false;
	bool wall = false;
	bool world_change = false;
	bool light_change = false;
	bool watermark = false;
	bool hitsound = false;
	int second_chams_xqz_esp_color_r = 255, second_chams_xqz_esp_color_g = 255, second_chams_xqz_esp_color_b = 255, second_chams_xqz_esp_color_a = 255;
	int gloss_effect_r = 255, gloss_effect_g, gloss_effect_b = 255, gloss_effect_a = 255;
	int gloss_effect_r_xqz = 255, gloss_effect_g_xqz = 255, gloss_effect_b_xqz = 255, gloss_effect_a_xqz = 255;
	int glow_esp_color_r = 255, glow_esp_color_g = 255, glow_esp_color_b = 255, glow_esp_color_a = 255;
	bool gloss_effect = false;
	bool gloss_effect_xqz = false;
	bool wireframe = false;
	bool wireframe_second = false;
	bool wireframe_team = false;
	bool wireframe_second_team = false;
	int chams_esp_color_r = 255, chams_esp_color_g = 255, chams_esp_color_b = 255, chams_esp_color_a = 255;
	int chams_xqz_esp_color_r = 255, chams_xqz_esp_color_g = 255, chams_xqz_esp_color_b = 255, chams_xqz_esp_color_a = 255;
	int chams_esp_color_r_team = 255, chams_esp_color_g_team = 255, chams_esp_color_b_team = 255, chams_esp_color_a_team = 255;
	int chams_xqz_esp_color_r_team = 255, chams_xqz_esp_color_g_team = 255, chams_xqz_esp_color_b_team = 255, chams_xqz_esp_color_a_team = 255;
	int chams_mode_enemy = 0;
	int model_mode = 0;
	int transparency = 0;
	int transparency_second = 0;
	bool visuals_enabled = false;
	bool visuals_world_enabled = false;
	int box_r = 255, box_g = 255, box_b = 255, box_a = 255;
	int name_r = 255, name_g = 255, name_b = 255, name_a = 255;
	int hp_r = 255, hp_g = 255, hp_b = 255, hp_a = 255;
	int ammo_r = 255, ammo_g = 255, ammo_b = 255, ammo_a = 255;
	int weapon_r = 255, weapon_g = 255, weapon_b = 255, weapon_a = 255;
	int zoom_r = 255, zoom_g = 255, zoom_b = 255, zoom_a = 255;
	int ar_r = 255, ar_g = 255, ar_b = 255, ar_a = 255;
	int fd_r = 255, fd_g = 255, fd_b = 255, fd_a = 255;
	int money_r = 255, money_g = 255, money_b = 255, money_a = 255;
	int weapon_box_r = 255, weapon_box_g = 255, weapon_box_b = 255, weapon_box_a = 255;
	int weapon_name_r = 255, weapon_name_g = 255, weapon_name_b = 255, weapon_name_a = 255;
	int weapon_ammo_r = 255, weapon_ammo_g = 255, weapon_ammo_b = 255, weapon_ammo_a = 255;
	int glow_weapon_r = 255, glow_weapon_g = 255, glow_weapon_b = 255, glow_weapon_a = 255;
	bool glow_enemies = false;
	bool weapon_draw_box = false;
	bool weapon_draw_name = false;
	bool weapon_draw_ammo = false;
	bool weapon_draw_glow = false;
	bool draw_box = false;
	bool draw_name = false;
	bool draw_health = false;
	bool draw_ammo = false;
	bool draw_weapon = false;
	bool draw_zoom_id = false;
	bool draw_hk_id = false;
	bool draw_fd_id = false;
	bool money = false;
	bool menu_optimize = false;

	Color gradient_menu_col;

	bool radius = false;

	int chams_mode_teammate = 0;
	bool chams_team = false;
	bool chams_xqz_team = false;

	bool gloss_effect_team = false;
	bool gloss_effect_xqz_team = false;

	float bloom = 0;
	float exposure = 0;
	float ambient = 0;

	int transparency_team = 0;
	int transparency_second_team = 0;

	int gloss_effect_r_team = 255, gloss_effect_g_team, gloss_effect_b_team = 255, gloss_effect_a_team = 255;
	int gloss_effect_r_xqz_team = 255, gloss_effect_g_xqz_team = 255, gloss_effect_b_xqz_team = 255, gloss_effect_a_xqz_team = 255;

	int change_world = 0;

	bool fov_arrows_enemy = false;
	int fov_arrows_enemy_size = 0;
	int fov_arrows_enemy_distance = 0;
	int fov_arrows_esp_color_r = 255, fov_arrows_esp_color_g = 255, fov_arrows_esp_color_b = 255, fov_arrows_esp_color_a = 255;

	bool bhop = false;
	int strafe_type = 0;

	int tp_key = -1;
	int thirdperson_distance = 0;

	bool aimbot_enabled = false;
	int test = 0;

	int damage[6];
	int hitchance[6];
	int head_scale[6];
	int body_scale[6];
	int damage_invis[6];

	int chams_mode_local = 0;
	bool wireframe_local = false;
	bool chams_local = false;
	bool second_wireframe_local = false;
	bool gloss_local = false;
	int chams_color_local_r = 255, chams_color_local_g = 255, chams_color_local_b = 255;
	int gloss_color_local_r = 255, gloss_color_local_g = 255, gloss_color_local_b = 255;

	int transparency_local = 0;
	int transparency_second_local = 0;

	bool silent = false;
	bool recoil_control = false;
	bool auto_revolver = false;

	bool ignore_limbs[6];
	int priority[6];

	int target_selection = 0;
	int max_fov = 0;

	int chams_mode_fake = 0;
	bool wireframe_fake = false;
	bool chams_fake = false;
	int chams_color_fake_r = 255, chams_color_fake_g = 255, chams_color_fake_b = 255;
	bool gloss_fake = false;
	bool second_wireframe_fake = false;
	int gloss_color_fake_r = 255, gloss_color_fake_g = 255, gloss_color_fake_b = 255;

	int transparency_fake = 0;
	int transparency_second_fake = 0;

	int yaw = 0;
	int pitch = 0;
	int jitter = 0;
	bool leg = false;

	int invert_key = -1;

	int dt_key = -1;
	int slow_key = -1;

	bool hold_tp = false;
	bool hold_invert = false;
	bool hold_dt = false;

	bool invert_real = false;

	int body_lean = 0;
	int inverted_body_lean = 0;

	int fake_mode = 0;
	int fake_left = 0;
	int fake_right = 0;
	bool invert_fake = false;

	bool fake_lag_start = false;
	int fakelag_mode = 0;
	int lag_limit = 0;

	int active_exploit = 0;

	int slow_walk = 0;

	bool baim_lethal = false;
	bool baim_air = false;
	
	bool interpolate = false;
	bool force = false;

};