#include "csgo_alt.h"
#include "utils.h"

__forceinline uint8_t* find_sig(const uint32_t offset, const char* signature, const uint32_t range = 0u)
{
	static auto pattern_to_bytes = [](const char* pattern) -> std::vector<int> {
		auto bytes = std::vector<int32_t>{ };
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				current++;

				if (*current == '?')
					current++;

				bytes.push_back(-1);
			}
			else
				bytes.push_back(static_cast<int32_t>(strtoul(current, &current, 0x10)));
		}

		return bytes;
	};

	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(offset);
	auto pattern_bytes = pattern_to_bytes(signature);
	const auto s = pattern_bytes.size();
	const auto d = pattern_bytes.data();

	for (auto i = 0ul; i < range - s; ++i) {
		auto found = true;

		for (auto j = 0ul; j < s; ++j)
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}

		if (found)
			return &scan_bytes[i];
	}

	return nullptr;
}

__forceinline uint8_t* find_module_sig(const char* name, const char* signature)
{
	const auto module = GetModuleHandleA(name);//get_module_handle(name);

	if (module) {
		const auto dos_header = PIMAGE_DOS_HEADER(module);
		const auto nt_headers = PIMAGE_NT_HEADERS(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

		return find_sig((DWORD)module, signature, nt_headers->OptionalHeader.SizeOfImage);
	}

	return nullptr;
}

namespace detour_hook
{

	void detour_hook::init()
	{

		static const auto c_cs_player_table = reinterpret_cast<uint32_t>(find_module_sig("client.dll",
			"55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C")) + 0x47;

		DWORD* pt = (DWORD*)*(DWORD*)(c_cs_player_table);
		detour_hook::orig_accumulate_layers = (detour_hook::accumulate_layers_fn)DetourFunction((PBYTE)pt[208], (PBYTE)detour_hook::accumulate_layers);
		detour_hook::original_build_transformations = (detour_hook::build_transformations_t)DetourFunction((PBYTE)pt[189], (PBYTE)detour_hook::build_transformations);
		detour_hook::orig_do_extra_bone_processing = (detour_hook::do_extra_bone_processing_t)DetourFunction((PBYTE)pt[197], (PBYTE)detour_hook::do_extra_bone_processing);
		detour_hook::origin_standard_blending_rules = (detour_hook::standard_blending_rules_t)DetourFunction((PBYTE)pt[205], (PBYTE)detour_hook::standard_blending_rules);

		static const auto _sbf = find_module_sig("client.dll", ("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C")) + 0x4E;
		DWORD* sbf = (DWORD*)*(DWORD*)(_sbf);

		detour_hook::original_setup_bones = (detour_hook::SetupBones_t)DetourFunction((PBYTE)sbf[13], (PBYTE)detour_hook::setup_bones);

	}

}