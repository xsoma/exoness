#include "csgo_hooks.h"
#include "main_entity.h"
#include "menu.h"

using do_post_screen_effects_t = void(__thiscall*)(void* thisptr, CViewSetup * setup);

void __fastcall hooks::post_screen_effects(void* thisptr, void* edx, CViewSetup* setup) {

	static auto original_fn = hooks::client_mode_hook->get_original<do_post_screen_effects_t>(44);

	for (auto i = 0; i < interfaces::glow->GetSize(); i++)
	{
		auto& glowObject = interfaces::glow->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<c_base_entity*>(glowObject.m_pEntity);
		auto m_pLocalPlayer = reinterpret_cast<c_base_entity*>(interfaces::entitylist->GetClientEntity(interfaces::engine->GetLocalPlayer()));

		if (!entity)
			continue;

		if (!m_pLocalPlayer)
			continue;

		if (glowObject.IsUnused())
			continue;

		bool is_local_player = entity == m_pLocalPlayer;
		bool is_teammate = m_pLocalPlayer->m_iTeamNum() == entity->m_iTeamNum() && !is_local_player;
		bool IsEnemy = entity->m_iTeamNum() != m_pLocalPlayer->m_iTeamNum();

		if (!is_teammate && !is_local_player) {
			if (c_config::get().glow_enemies) {
				auto class_id = entity->GetClientClass()->m_ClassID;

				switch (class_id)
				{
				default:
					glowObject.m_flAlpha = 0.0f;
					break;
				case 40:
					glowObject.m_nGlowStyle = 0;
					glowObject.m_flAlpha = 255.f / 255.f;
					break;
				}

				glowObject.m_flRed = c_config::get().glow_esp_color_r / 255.0f;
				glowObject.m_flGreen = c_config::get().glow_esp_color_g / 255.0f;
				glowObject.m_flBlue = c_config::get().glow_esp_color_b / 255.0f;
				glowObject.m_bRenderWhenOccluded = true;
				glowObject.m_bRenderWhenUnoccluded = false;
			}
		}

		if (c_config::get().weapon_draw_glow && entity->IsWeapon())
		{
			glowObject.m_nGlowStyle = 0;
			glowObject.m_flAlpha = 1.f;
			glowObject.m_flRed = c_config::get().glow_weapon_r / 255.0f;
			glowObject.m_flGreen = c_config::get().glow_weapon_g / 255.0f;
			glowObject.m_flBlue = c_config::get().glow_weapon_b / 255.0f;
			glowObject.m_bRenderWhenOccluded = true;
			glowObject.m_bRenderWhenUnoccluded = false;
		}
	}

	return original_fn(thisptr, setup);
}