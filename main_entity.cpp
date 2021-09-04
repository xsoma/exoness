#include "main_entity.h"
#include "mathematics.h"

bool c_base_entity::is_alive()
{
	return m_lifeState() == LIFE_ALIVE;
}

player_info_t c_base_entity::GetPlayerInfo()
{
	player_info_t info;
	interfaces::engine->GetPlayerInfo(EntIndex(), &info);
	return info;
}

int c_base_entity::sequence_activity(int sequence)
{
	auto hdr = interfaces::modelinfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(utils::pattern_scan("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));
	return get_sequence_activity(this, hdr, sequence);
}

AnimationLayer* c_base_entity::get_animlayers()
{
	return *(AnimationLayer**)((DWORD)this + 0x2980);
}

bool c_base_entity::is_player()
{
	return call_virtual< bool(__thiscall*) (c_base_entity*) >(this, 157) (this);
}

Vector c_base_entity::get_eye_pos()
{
	return m_vecOrigin() + m_vecViewOffset();
}

CCSGOPlayerAnimState* c_base_entity::get_animation_state()
{
	return *(CCSGOPlayerAnimState**)((DWORD)this + 0x3914);
}

CUserCmd*& c_base_entity::current_command()
{
	static auto current_command = *reinterpret_cast<uint32_t*> (utils::pattern_scan("client.dll",
		"89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *reinterpret_cast<CUserCmd**> (reinterpret_cast<uintptr_t> (this) + current_command);
}

void c_base_entity::set_abs_origin(Vector origin)
{
	using Fn = void(__thiscall*)(void*, const Vector & origin);
	static Fn AbsOrigin = (Fn)utils::find_signature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

	AbsOrigin(this, origin);
}

void c_base_entity::update_animation_state(CCSGOPlayerAnimState* state, Vector angle)
{
	static auto UpdateAnimState = utils::pattern_scan(
		"client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

void c_base_entity::reset_animation_state(CCSGOPlayerAnimState* state)
{
	using ResetAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*);
	static auto ResetAnimState = (ResetAnimState_t)utils::pattern_scan("client.dll", "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void c_base_entity::create_animation_state(CCSGOPlayerAnimState* state)
{
	using CreateAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*, c_base_entity*);
	static auto CreateAnimState = (CreateAnimState_t)utils::pattern_scan("client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
	if (!CreateAnimState)
		return;

	CreateAnimState(state, this);
}

void c_base_entity::invalidate_bone_cache(void) {
	static auto invalidate_bone_cache_fn = utils::pattern_scan("client.dll",
		"80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81");
	reinterpret_cast<void(__fastcall*) (void*)> (invalidate_bone_cache_fn) (this);
}

bool c_base_entity::is_valid_player() {

	if (!this)
		return false;

	if (this->IsDormant() || !this->is_alive())
		return false;

	if (game::main.get_local()->m_iTeamNum() == this->m_iTeamNum())
		return false;

	if (this->GetClientClass()->m_ClassID != 40)
		return false;

	if (this == game::main.get_local())
		return false;

	if (this->m_bGunGameImmunity())
		return false;


	return true;
}

float_t c_base_entity::m_flSpawnTime()
{
	return *(float_t*)((uintptr_t)this + 0xA360);
}

void c_base_entity::set_abs_angles(const Vector& angles) {
	if (!this)
		return;

	static auto _set_abs_angles = (void(__thiscall*)(void*, const Vector&)) utils::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");

	_set_abs_angles(this, angles);
}

Vector c_base_entity::hitbox_position(int hitbox_id) {
	matrix3x4_t matrix[128];

	if (this->SetupBones(matrix, 128, 0x100, 0.f)) {
		studiohdr_t* hdr = interfaces::modelinfo->GetStudiomodel(this->GetModel());

		if (hdr) {
			mstudiobbox_t* hitbox = hdr->GetHitboxSet(0)->GetHitbox(hitbox_id);

			if (hitbox) {
				Vector
					min = Vector{ },
					max = Vector{ };

				math::vector_transform(hitbox->bbmin, matrix[hitbox->bone], min);
				math::vector_transform(hitbox->bbmax, matrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}

	return Vector{ };
}

bool c_base_combat_weapon::is_empty() {
	return m_iClip1() <= 0;
}

bool c_base_combat_weapon::can_fire() {
	if (is_empty()) {
		return false;
	}

	if (!game::main.get_local()) {
		return false;
	}

	return (m_flNextPrimaryAttack() < interfaces::global_vars->curtime) && (game::main.get_local()->m_flNextAttack() < interfaces::global_vars->curtime);
}

bool c_base_combat_weapon::can_fire_post_pone()
{
	float rdyTime = m_flPostponeFireReadyTime();

	if (rdyTime > 0 && rdyTime < interfaces::global_vars->curtime)
		return true;

	return false;
}

bool c_base_combat_weapon::is_non_aim()
{
	int idx = m_iItemDefinitionIndex();

	return (idx == WEAPON_C4 || idx == WEAPON_KNIFE || idx == WEAPON_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL || idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}

bool c_base_combat_weapon::is_grenade()
{
	int idx = m_iItemDefinitionIndex();

	return (idx == idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}

bool c_base_combat_weapon::is_pistol()
{
	int WeapID = m_iItemDefinitionIndex();

	return WeapID == ItemDefinitionIndex::WEAPON_DEAGLE || WeapID == ItemDefinitionIndex::WEAPON_ELITE || WeapID == ItemDefinitionIndex::WEAPON_FIVESEVEN || WeapID == ItemDefinitionIndex::WEAPON_P250 ||
		WeapID == ItemDefinitionIndex::WEAPON_GLOCK || WeapID == ItemDefinitionIndex::WEAPON_USP_SILENCER || WeapID == ItemDefinitionIndex::WEAPON_CZ75A || WeapID == ItemDefinitionIndex::WEAPON_HKP2000 || WeapID == ItemDefinitionIndex::WEAPON_TEC9 || WeapID == ItemDefinitionIndex::WEAPON_REVOLVER;
}