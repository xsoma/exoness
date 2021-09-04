#include "csgo_hooks.h"
#include "menu.h"
#include "utils.h"
#include <vector>

typedef void(__fastcall* scene_end_t) (void*, void*);

static auto linegoesthrusmoke = utils::find_by_pattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");

void __fastcall hooks::scene_end(void* ecx, void* edx)
{
	static auto o_scene_end = hooks::render_view_hook->get_original<scene_end_t>(9);

	if (c_config::get().remove[2]) {
	
		std::vector<const char*> vistasmoke_wireframe =
		{
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
		};

		std::vector<const char*> vistasmoke_nodraw =
		{
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		};

		for (auto mat_s : vistasmoke_wireframe)
		{
			IMaterial* mat = interfaces::materialsystem->FindMaterial(mat_s, "Other textures");
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true); //wireframe
		}

		for (auto mat_n : vistasmoke_nodraw)
		{
			IMaterial* mat = interfaces::materialsystem->FindMaterial(mat_n, "Other textures");
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}

		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		*(int*)(smokecout) = 0;
	}

	o_scene_end(ecx, edx);
}
