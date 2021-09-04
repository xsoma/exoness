#include "utils.h"
#include <thread>
#include <Psapi.h>
#include "mathematics.h"
#include "main_entity.h"
#include "prediction.h"

namespace utils {

	int epoch_time() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	unsigned long pattern_scan(const char* module_name, const char* signature) {
		MODULEINFO module_info;

		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module_name), &module_info, sizeof(MODULEINFO));

		DWORD start_addr = (DWORD)module_info.lpBaseOfDll;
		DWORD end_addr = start_addr + module_info.SizeOfImage;
		const char* pat = signature;

		DWORD first_match = 0;

		for (DWORD cur = start_addr; cur < end_addr; cur++) {
			if (!*pat) return first_match;
			if (*(PBYTE)pat == '\?' || *(BYTE*)cur == GETBYTE(pat)) {
				if (!first_match) first_match = cur;
				if (!pat[2]) return first_match;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = signature;
				first_match = 0;
			}
		}

		return NULL;
	}

	DWORD find_by_pattern(const char* module_name, const BYTE* mask, const char* mask_string)
	{
		/// Get module address
		const unsigned int module_address = reinterpret_cast<unsigned int>(GetModuleHandle(module_name));

		/// Get module information to the size
		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(module_address), &module_info, sizeof(MODULEINFO));

		auto IsCorrectMask = [](const unsigned char* data, const unsigned char* mask, const char* mask_string) -> bool
		{
			for (; *mask_string; ++mask_string, ++mask, ++data)
				if (*mask_string == 'x' && *mask != *data)
					return false;

			return (*mask_string) == 0;
		};

		/// Iterate until we find a matching mask
		for (unsigned int c = 0; c < module_info.SizeOfImage; c += 1)
		{
			/// does it match?
			if (IsCorrectMask(reinterpret_cast<unsigned char*>(module_address + c), mask, mask_string))
				return (module_address + c);
		}

		return 0;
	}

	uintptr_t find_signature(const char* szModule, const char* szSignature)
	{
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(szModule);
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GETBYTE(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;
			}
			else
			{
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	bool world_to_screen(const Vector& origin, Vector2D& screen)
	{
		const auto screenTransform = [&origin, &screen]() -> bool
		{
			static std::uintptr_t pViewMatrix;
			if (!pViewMatrix)
			{
				pViewMatrix = static_cast<std::uintptr_t>(find_signature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
				pViewMatrix += 3;
				pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
				pViewMatrix += 176;
			}

			const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
			screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
			screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];

			float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

			if (w < 0.001f)
			{
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if (!screenTransform())
		{
			int iScreenWidth, iScreenHeight;
			interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);

			screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
			screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

			return true;
		}
		return false;
	}

	bool world_to_screen_vector(const Vector& origin, Vector& screen)
	{
		const auto screenTransform = [&origin, &screen]() -> bool
		{
			static std::uintptr_t pViewMatrix;
			if (!pViewMatrix)
			{
				pViewMatrix = static_cast<std::uintptr_t>(find_signature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
				pViewMatrix += 3;
				pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
				pViewMatrix += 176;
			}

			const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
			screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
			screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];

			float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

			if (w < 0.001f)
			{
				screen.x *= 100000;
				screen.y *= 100000;
				return true;
			}

			float invw = 1.f / w;
			screen.x *= invw;
			screen.y *= invw;

			return false;
		};

		if (!screenTransform())
		{
			int iScreenWidth, iScreenHeight;
			interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);

			screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
			screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

			return true;
		}
		return false;
	}

	void clip_ray_to_player(const Vector& src, const Vector& end, const uint32_t mask, c_base_entity* player, ITraceFilter* filter, trace_t* t)
	{
		if (!player || player->IsDormant())
			return;

		if (filter && !filter->ShouldHitEntity(player, mask))
			return;

		trace_t t_new{};
		Ray_t r{};
		r.Init(src, end);

		interfaces::trace->ClipRayToEntity(r, mask, player, &t_new);
		if (t_new.fraction < t->fraction)
			*t = t_new;
	}

	void trace_line(const Vector& vec_start, const Vector& vec_end, const unsigned int n_mask, c_base_entity* p_cs_ignore, trace_t* p_trace)
	{
		Ray_t ray;
		ray.Init(vec_start, vec_end);

		if (p_cs_ignore)
		{
			CTraceFilter filter;
			filter.pSkip = p_cs_ignore;
			interfaces::trace->TraceRay(ray, n_mask, &filter, p_trace);
			return;
		}

		CTraceFilterWorldOnly filter;
		interfaces::trace->TraceRay(ray, n_mask, &filter, p_trace);
	}

	float get_estimate_server_time(CUserCmd* cmd)
	{
		const auto v1 = static_cast<INetChannelInfo*> (interfaces::engine->GetNetChannelInfo());

		const auto v3 = v1->GetAvgLatency(INetChannelInfo::LOCALPLAYER);
		const auto v4 = v1->GetAvgLatency(INetChannelInfo::GENERIC);

		return v3 + v4 + ticks_to_time(1) + ticks_to_time(cmd->tick_count);
	}

	bool is_visible(Vector pos_notangles, bool smokecheck)
	{
		auto start = c_prediction::get().unpredicted_eye_pos;
		auto angle = math::calculate_angle(start, pos_notangles);
		auto end = Vector{ };

		if (!game::main.get_local()->is_alive() || game::main.get_local()->IsDormant() || !interfaces::engine->IsConnected())
			return false;

		angle = math::i_angle_t_vector_i(angle);
		angle *= game::main.get_local()->m_hActiveWeapon().Get()->GetCSWeaponData()->flRange;
		end = start + angle;

		trace_t trace;
		CTraceFilter filter;
		filter.pSkip = game::main.get_local();
		Ray_t ray; ray.Init(start, end);

		static auto line_goes_through_smoke = (bool(__cdecl*)(Vector, Vector, int16_t))(utils::pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

		if (line_goes_through_smoke(start, end, 1) && smokecheck)
			return false;

		interfaces::trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		auto hit_entity = (c_base_entity*)(trace.hit_entity);

		if (!hit_entity || !hit_entity->is_player() || !hit_entity->is_alive() || hit_entity->IsDormant() || hit_entity->m_iTeamNum() == game::main.get_local()->m_iTeamNum())
			return false;

		return true;
	}

	void fix_movement(Vector& oldang, CUserCmd * cmd)
	{
		Vector vMovements(cmd->forwardmove, cmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = cmd->viewangles;
		aRealDir.Clamp();

		math::angle_vectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		math::vector_normalize(vRealF);
		math::vector_normalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		math::angle_vectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		math::vector_normalize(vWishF);
		math::vector_normalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * cmd->forwardmove + vWishR[0] * cmd->sidemove;
		vWishVel[1] = vWishF[1] * cmd->forwardmove + vWishR[1] * cmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		cmd->forwardmove = x;
		cmd->sidemove = y;
	}

	bool is_any_visible(const Vector& start, const Vector& end, c_base_entity* player, c_base_entity* local)
	{
		Ray_t ray;
		trace_t tr;
		ray.Init(start, end);

		CTraceFilter filter;
		filter.pSkip = local;

		interfaces::trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		if (tr.hit_entity == player || tr.fraction > 0.97f)
		{
			return true;
		}

		return false;
	}

	bool method_key_bind(bool hold, int key)
	{
		if (hold)
		{
			if (GetAsyncKeyState(key))
				return true;
			else
				return false;
		}
		else
		{
			if (GetKeyState(key))
				return true;
			else
				return false;
		}
	}


}