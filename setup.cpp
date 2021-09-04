#include "menu.h"
#include "setup.h"
#include "config.h"

void CConfig::Setup()
{
	setup_value(c_config::get().menu_optimize, false, "menu", "menu_optimize");

	setup_value(c_config::get().menu_color_r, 255, "menu", "menu_setup_r");
	setup_value(c_config::get().menu_color_g, 102, "menu", "menu_setup_g");
	setup_value(c_config::get().menu_color_b, 179, "menu", "menu_setup_b");

	setup_value(c_config::get().visuals_enabled, false, "visuals", "visuals_enabled");
	setup_value(c_config::get().visuals_world_enabled, false, "visuals", "visuals_world_enabled");

	setup_value(c_config::get().draw_box, false, "visuals", "visuals_box");
	setup_value(c_config::get().draw_name, false, "visuals", "visuals_name");
	setup_value(c_config::get().draw_health, false, "visuals", "visuals_hp");
	setup_value(c_config::get().draw_weapon, false, "visuals", "visuals_weapon");
	setup_value(c_config::get().draw_ammo, false, "visuals", "visuals_ammo");
	setup_value(c_config::get().glow_enemies, false, "visuals", "visauls_glow");

	setup_value(c_config::get().weapon_draw_box, false, "visuals", "visuals_weapon_box");
	setup_value(c_config::get().weapon_draw_name, false, "visuals", "visuals_weapon_name");
	setup_value(c_config::get().weapon_draw_glow, false, "visuals", "visuals_glow_weapon");
	setup_value(c_config::get().weapon_draw_ammo, false, "visuals", "visuals_weapon_ammo");

	setup_value(c_config::get().weapon_box_r, 255, "visuals", "visuals_weapon_box_r");
	setup_value(c_config::get().weapon_box_g, 255, "visuals", "visuals_weapon_box_g");
	setup_value(c_config::get().weapon_box_b, 255, "visuals", "visuals_weapon_box_b");

	setup_value(c_config::get().weapon_name_r, 255, "visuals", "visuals_weapon_name_r");
	setup_value(c_config::get().weapon_name_g, 255, "visuals", "visuals_weapon_name_g");
	setup_value(c_config::get().weapon_name_b, 255, "visuals", "visuals_weapon_name_b");

	setup_value(c_config::get().glow_weapon_r, 255, "visuals", "visuals_weapon_glow_r");
	setup_value(c_config::get().glow_weapon_g, 255, "visuals", "visuals_weapon_glow_g");
	setup_value(c_config::get().glow_weapon_b, 255, "visuals", "visuals_weapon_glow_b");

	setup_value(c_config::get().weapon_ammo_r, 255, "visuals", "visuals_weapon_glow_r");
	setup_value(c_config::get().weapon_ammo_g, 255, "visuals", "visuals_weapon_glow_g");
	setup_value(c_config::get().weapon_ammo_b, 255, "visuals", "visuals_weapon_glow_b");

	setup_value(c_config::get().box_r, 255, "visuals", "visuals_box_r");
	setup_value(c_config::get().box_g, 255, "visuals", "visuals_box_g");
	setup_value(c_config::get().box_b, 255, "visuals", "visuals_box_b");

	setup_value(c_config::get().name_r, 255, "visuals", "visuals_name_r");
	setup_value(c_config::get().name_g, 255, "visuals", "visuals_name_g");
	setup_value(c_config::get().name_b, 255, "visuals", "visuals_name_b");

	setup_value(c_config::get().hp_r, 255, "visuals", "visuals_hp_r");
	setup_value(c_config::get().hp_g, 255, "visuals", "visuals_hp_g");
	setup_value(c_config::get().hp_b, 255, "visuals", "visuals_hp_b");

	setup_value(c_config::get().ammo_r, 255, "visuals", "visuals_ammo_r");
	setup_value(c_config::get().ammo_g, 255, "visuals", "visuals_ammo_g");
	setup_value(c_config::get().ammo_b, 255, "visuals", "visuals_ammo_b");

	setup_value(c_config::get().weapon_r, 255, "visuals", "visuals_weapon_r");
	setup_value(c_config::get().weapon_g, 255, "visuals", "visuals_weapon_g");
	setup_value(c_config::get().weapon_b, 255, "visuals", "visuals_weapon_b");

	setup_value(c_config::get().glow_esp_color_r, 255, "visuals", "visuals_glow_r");
	setup_value(c_config::get().glow_esp_color_g, 255, "visuals", "visuals_glow_g");
	setup_value(c_config::get().glow_esp_color_b, 255, "visuals", "visuals_glow_b");

	setup_value(c_config::get().transparency, 255, "chams", "chams_trp");
	setup_value(c_config::get().transparency_second, 255, "chams", "chams_trp_second");
	setup_value(c_config::get().transparency_team, 255, "chams", "chams_trp_team");
	setup_value(c_config::get().transparency_second_team, 255, "chams", "chams_trp_team_second");

	setup_value(c_config::get().chams_mode_enemy, 0, "chams", "chams_mode");
	setup_value(c_config::get().chams_mode_teammate, 0, "chams", "chams_mode_team");

	setup_value(c_config::get().wireframe, false, "chams", "chams_wire");
	setup_value(c_config::get().wireframe_team, false, "chams", "chams_wire_team");
	setup_value(c_config::get().wireframe_second, false, "chams", "chams_wire_second");
	setup_value(c_config::get().wireframe_second_team, false, "chams", "chams_wire_team_second");

	setup_value(c_config::get().chams_enemies, false, "chams", "chams_enemy");
	setup_value(c_config::get().chams_team, false, "chams", "chams_team");
	setup_value(c_config::get().chams_xqz_enemies, false, "chams", "chams_xqz_enemy");
	setup_value(c_config::get().chams_xqz_team, false, "chams", "chams_xqz_team");

	setup_value(c_config::get().gloss_effect, false, "chams", "chams_enemy_second");
	setup_value(c_config::get().gloss_effect_team, false, "chams", "chams_team_second");
	setup_value(c_config::get().gloss_effect_xqz, false, "chams", "chams_xqz_enemy_second");
	setup_value(c_config::get().gloss_effect_xqz_team, false, "chams", "chams_xqz_team_second");

	setup_value(c_config::get().chams_esp_color_r_team, 255, "chams", "team_chams_clr_r");
	setup_value(c_config::get().chams_esp_color_g_team, 255, "chams", "team_chams_clr_g");
	setup_value(c_config::get().chams_esp_color_b_team, 255, "chams", "team_chams_clr_b");

	setup_value(c_config::get().chams_xqz_esp_color_r_team, 255, "chams", "team_chams_clr_r_xqz");
	setup_value(c_config::get().chams_xqz_esp_color_g_team, 255, "chams", "team_chams_clr_g_xqz");
	setup_value(c_config::get().chams_xqz_esp_color_b_team, 255, "chams", "team_chams_clr_b_xqz");

	setup_value(c_config::get().chams_esp_color_r, 255, "chams", "enemy_chams_clr_r");
	setup_value(c_config::get().chams_esp_color_g, 255, "chams", "enemy_chams_clr_g");
	setup_value(c_config::get().chams_esp_color_b, 255, "chams", "enemy_chams_clr_b");

	setup_value(c_config::get().chams_xqz_esp_color_r, 255, "chams", "enemy_chams_clr_r_xqz");
	setup_value(c_config::get().chams_xqz_esp_color_g, 255, "chams", "enemy_chams_clr_g_xqz");
	setup_value(c_config::get().chams_xqz_esp_color_b, 255, "chams", "enemy_chams_clr_b_xqz");

	setup_value(c_config::get().gloss_effect_r, 255, "chams", "enemy_chams_gloss_r");
	setup_value(c_config::get().gloss_effect_g, 255, "chams", "enemy_chams_gloss_g");
	setup_value(c_config::get().gloss_effect_b, 255, "chams", "enemy_chams_gloss_b");

	setup_value(c_config::get().gloss_effect_r_xqz, 255, "chams", "enemy_chams_gloss_r_xqz");
	setup_value(c_config::get().gloss_effect_g_xqz, 255, "chams", "enemy_chams_gloss_g_xqz");
	setup_value(c_config::get().gloss_effect_b_xqz, 255, "chams", "enemy_chams_gloss_b_xqz");

	setup_value(c_config::get().gloss_effect_r_team, 255, "chams", "team_chams_gloss_r");
	setup_value(c_config::get().gloss_effect_g_team, 255, "chams", "team_chams_gloss_g");
	setup_value(c_config::get().gloss_effect_b_team, 255, "chams", "team_chams_gloss_b");

	setup_value(c_config::get().gloss_effect_r_xqz_team, 255, "chams", "team_chams_gloss_r_xqz");
	setup_value(c_config::get().gloss_effect_g_xqz_team, 255, "chams", "team_chams_gloss_g_xqz");
	setup_value(c_config::get().gloss_effect_b_xqz_team, 255, "chams", "team_chams_gloss_b_xqz");


	/////////////////////////////////////////////////////////////////////////////////////////////

	setup_value(c_config::get().aimbot_enabled, false, "rage", "rage_enabled");
	setup_value(c_config::get().target_selection, 0, "rage", "rage_select");
	setup_value(c_config::get().max_fov, 0, "rage", "rage_fov");

	for (int i = 0; i < 6; i++) {
		std::string start = "Ragebot.";
		const char* names[7] = { "pistols", "deagle", "rifles", "smgs", "snipers", "autos", "shotguns" };
		start += names[i];

		setup_value(c_config::get().head_scale[i], 0, "Aimbot", start + "aimbot_headpointscale");
		setup_value(c_config::get().body_scale[i], 0, "Aimbot", start + "aimbot_bodypointscale");
		setup_value(c_config::get().damage[i], 0, "Aimbot", start + "aimbot_hitchance_new");
		setup_value(c_config::get().hitchance[i], 0, "Aimbot", start + "aimbot_damage_new");
		setup_value(c_config::get().priority[i], 0, "Aimbot", start + "aimbot_limiter");
		
	}

	setup_value(c_config::get().baim_air, false, "rage", "rage_air_b");
	setup_value(c_config::get().baim_lethal, false, "rage", "rage_lethal_b");

	setup_value(c_config::get().yaw, 0, "aa", "aa_yaw");
	setup_value(c_config::get().pitch, 0, "aa", "aa_x");
	setup_value(c_config::get().jitter, 0, "aa", "aa_jitter");
	setup_value(c_config::get().body_lean, 0, "aa", "aa_lean");
	setup_value(c_config::get().inverted_body_lean, 0, "aa", "aa_in_lean");
	setup_value(c_config::get().invert_real, false, "aa", "aa_invert");
	setup_value(c_config::get().invert_fake, false, "aa", "aa_invert_fake");
	setup_value(c_config::get().leg, false, "aa", "aa_fix");

	setup_value(c_config::get().fake_mode, 0, "aa", "aa_yaw_f");
	setup_value(c_config::get().fake_left, 0, "aa", "aa_yaw_f_left");
	setup_value(c_config::get().fake_right, 0, "aa", "aa_yaw_f_right");

	setup_value(c_config::get().active_exploit, 0, "exploit", "exploits_run");

	setup_value(c_config::get().fake_lag_start, false, "lag", "fakelag_start");
	setup_value(c_config::get().fakelag_mode, 0, "lag", "lag_mode");
	setup_value(c_config::get().lag_limit, 0, "lag", "lag_limited");

	setup_value(c_config::get().fov_arrows_enemy, false, "esp", "fov_arrows_start");
	setup_value(c_config::get().fov_arrows_enemy_distance, 0, "esp", "fov_arrows_start_dist");
	setup_value(c_config::get().fov_arrows_enemy_size, 0, "esp", "fov_arrows_start_size");

	setup_value(c_config::get().fov_arrows_esp_color_r, 255, "esp", "fov_arrows_rgb_r");
	setup_value(c_config::get().fov_arrows_esp_color_g, 255, "esp", "fov_arrows_rgb_g");
	setup_value(c_config::get().fov_arrows_esp_color_b, 255, "esp", "fov_arrows_rgb_b");
	
	setup_value(c_config::get().flags, false, 2, "esp", "esp_flags");
	setup_value(c_config::get().radius, false, "esp", "esp_radius");

	setup_value(c_config::get().dt_key, -1, "key_bind", "key_bind_dt");
	setup_value(c_config::get().invert_key, -1, "key_bind", "key_bind_invert");
	setup_value(c_config::get().slow_key, -1, "key_bind", "key_bind_slow");
	setup_value(c_config::get().tp_key, -1, "key_bind", "key_bind_th");

	setup_value(c_config::get().hold_dt, false, "key_bind", "hold_dt");
	setup_value(c_config::get().hold_invert, false, "key_bind", "hold_invert");
	setup_value(c_config::get().hold_tp, false, "key_bind", "hold_thp");

	setup_value(c_config::get().hitsound, false, "misc", "misc_hitsound");
	setup_value(c_config::get().bhop, false, "misc", "misc_bhop");
	setup_value(c_config::get().strafe_type, 0, "misc", "misc_strafe_type");
	setup_value(c_config::get().slow_walk, 0, "misc", "misc_slow_walk");

	setup_value(c_config::get().exposure, 0, "world", "world_exp");
	setup_value(c_config::get().ambient, 0, "world", "world_amb");
	setup_value(c_config::get().bloom, 0, "world", "world_bloom");

	setup_value(c_config::get().light_change, false, "world", "world_change_light");
	setup_value(c_config::get().world_change, false, "world", "world_change");

	setup_value(c_config::get().sky, false, "world", "world_sky");
	setup_value(c_config::get().wall, false, "world", "world_wall");

	setup_value(c_config::get().sky_color_r, 255, "world", "world_sky_r");
	setup_value(c_config::get().sky_color_g, 255, "world", "world_sky_g");
	setup_value(c_config::get().sky_color_b, 255, "world", "world_sky_b");

	setup_value(c_config::get().wall_color_r, 255, "world", "world_wall_r");
	setup_value(c_config::get().wall_color_g, 255, "world", "world_wall_g");
	setup_value(c_config::get().wall_color_b, 255, "world", "world_wall_b");

	setup_value(c_config::get().remove_zoom, false, "local", "local_remove_zoom");
	setup_value(c_config::get().fov, 0, "local", "local_field_of");
	setup_value(c_config::get().thirdperson_distance, 0, "local", "local_third_field_of");
	setup_value(c_config::get().remove, 0, 5, "local", "local_removals");

	setup_value(c_config::get().transparency_local, 255, "chams", "chams_local_trans");
	setup_value(c_config::get().c_config::get().transparency_second_local, 255, "chams", "chams_local_trans_second");

	setup_value(c_config::get().transparency_fake, 255, "chams", "chams_fake_trans");
	setup_value(c_config::get().c_config::get().transparency_second_fake, 255, "chams", "chams_fake_trans_second");

	setup_value(c_config::get().chams_mode_local, 0, "chams", "mode_chams_local");
	setup_value(c_config::get().chams_mode_fake, 0, "chams", "mode_chams_fake");

	setup_value(c_config::get().wireframe_local, false, "chams", "chams_local_wire");
	setup_value(c_config::get().wireframe_fake, false, "chams", "chams_fake_wire");

	setup_value(c_config::get().second_wireframe_local, false, "chams", "chams_local_wire_second");
	setup_value(c_config::get().second_wireframe_fake, false, "chams", "chams_fake_wire_second");

	setup_value(c_config::get().chams_color_local_r, 255, "chams", "chams_local_r");
	setup_value(c_config::get().chams_color_local_g, 255, "chams", "chams_local_g");
	setup_value(c_config::get().chams_color_local_b, 255, "chams", "chams_local_b");

	setup_value(c_config::get().chams_local, false, "chams", "chams_local_start");
	setup_value(c_config::get().chams_fake, false, "chams", "chams_fake_start");

	setup_value(c_config::get().gloss_local, false, "chams", "gloss_chams_local_start");
	setup_value(c_config::get().gloss_fake, false, "chams", "gloss_chams_fake_start");

	setup_value(c_config::get().gloss_color_local_r, 255, "chams", "gloss_chams_local_r");
	setup_value(c_config::get().gloss_color_local_g, 255, "chams", "gloss_chams_local_g");
	setup_value(c_config::get().gloss_color_local_b, 255, "chams", "gloss_chams_local_b");

	setup_value(c_config::get().gloss_color_fake_r, 255, "chams", "gloss_chams_fake_r");
	setup_value(c_config::get().gloss_color_fake_g, 255, "chams", "gloss_chams_fake_g");
	setup_value(c_config::get().gloss_color_fake_b, 255, "chams", "gloss_chams_fake_b");

	setup_value(c_config::get().chams_color_fake_r, 255, "chams", "chams_fake_r");
	setup_value(c_config::get().chams_color_fake_g, 255, "chams", "chams_fake_g");
	setup_value(c_config::get().chams_color_fake_b, 255, "chams", "chams_fake_b");


}

void CConfig::setup_value(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}

void CConfig::setup_value(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(category, name, &value));
}

void CConfig::setup_value(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(category, name, &value));
}

void CConfig::setup_value(bool* value, bool def, int size, std::string category, std::string name)
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(category, name, &value[c]));
	}
}

void CConfig::Save(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\exoness\\";
		file = std::string(path) + "\\exoness\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\exoness\\";
		file = std::string(path) + "\\exoness\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* csgo_config = new CConfig();