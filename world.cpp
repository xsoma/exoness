#include "world.h"
#include "menu.h"

void world::on_fsn_state ( ClientFrameStage_t stage )
{
    if (!c_config::get().light_change)
        return;

	if ( stage == FRAME_RENDER_START )
	{
        for (auto i = 1; i <= interfaces::entitylist->GetHighestEntityIndex( ); ++i)
        {
            auto entity = reinterpret_cast < c_base_entity* > ( interfaces::entitylist->GetClientEntity(i) );

            if ( !entity )
                continue;

            if ( entity->GetClientClass()->m_ClassID == 69)
            {  
                entity->m_bUseCustomAutoExposureMin() = true;
                entity->m_bUseCustomAutoExposureMax() = true;
                entity->m_flCustomAutoExposureMin() = c_config::get().exposure;
                entity->m_flCustomAutoExposureMax() = c_config::get().exposure;
                entity->m_flCustomBloomScale() = c_config::get().bloom;
            }
        }
	} 
}

void world::on_paint_traverse()
{
    
    if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
        return;

    if (!game::main.get_local())
        return;

    static int clr_r, clr_g, clr_b;
    static int clr_r_sky, clr_g_sky, clr_b_sky;
    static int clr_r_prop, clr_g_prop, clr_b_prop, clr_a_prop;

    if (c_config::get().world_change)
    {
        if (c_config::get().wall)
        {
            if (clr_r != c_config::get().wall_color_r || clr_g != c_config::get().wall_color_g || clr_b != c_config::get().wall_color_b)
            {
                for (MaterialHandle_t i = interfaces::materialsystem->FirstMaterial(); i != interfaces::materialsystem->InvalidMaterial(); i = interfaces::materialsystem->NextMaterial(i))
                {
                    IMaterial* pMaterial = interfaces::materialsystem->GetMaterial(i);

                    if (!pMaterial)
                        continue;

                    const char* group = pMaterial->GetTextureGroupName();
                    const char* name = pMaterial->GetName();

                    if (strstr(group, "World textures"))
                    {
                        pMaterial->ColorModulate(c_config::get().wall_color_r / 255.f, c_config::get().wall_color_g / 255.f, c_config::get().wall_color_b / 255.f);
                    }
                }

                clr_r = c_config::get().wall_color_r;
                clr_g = c_config::get().wall_color_g;
                clr_b = c_config::get().wall_color_b;
            }
        }

        if (c_config::get().sky)
        {
            if (clr_r_sky != c_config::get().sky_color_r || clr_g_sky != c_config::get().sky_color_g || clr_b_sky != c_config::get().sky_color_b)
            {
                for (MaterialHandle_t i = interfaces::materialsystem->FirstMaterial(); i != interfaces::materialsystem->InvalidMaterial(); i = interfaces::materialsystem->NextMaterial(i))
                {
                    IMaterial* pMaterial = interfaces::materialsystem->GetMaterial(i);

                    if (!pMaterial)
                        continue;

                    const char* group = pMaterial->GetTextureGroupName();
                    const char* name = pMaterial->GetName();

                    if (strstr(group, "SkyBox"))
                    {
                        pMaterial->ColorModulate(c_config::get().sky_color_r / 255.f, c_config::get().sky_color_g / 255.f, c_config::get().sky_color_b / 255.f);
                    }
                }

                clr_r_sky = c_config::get().sky_color_r;
                clr_g_sky = c_config::get().sky_color_g;
                clr_b_sky = c_config::get().sky_color_b;
            }
        }
    }
     
}