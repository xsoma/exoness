#include "csgo_hooks.h"
#include "utils.h"
#include <intrin.h>
#include "globals.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef bool(__thiscall* write_user_cmd_t)(void* ecx, int nSlot, void* buf, int from, int to, bool isNewCmd);

bool __fastcall hooks::write_user_cmd(void* ECX, void* EDX, int nSlot, bf_write* buf, int from, int to, bool isNewCmd)
{
	static DWORD WriteUsercmdDeltaToBufferReturn = (DWORD)utils::find_signature("engine.dll", "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84");
	static auto sendmovecall = utils::find_signature("engine.dll", "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?");
	static auto ofunct = hooks::client_hook->get_original<write_user_cmd_t>(24);

	if (_ReturnAddress() != (void*)sendmovecall || game::main.m_shift <= 0)
	{
		return ofunct(ECX, nSlot, buf, from, to, isNewCmd);
	} 

	if (from != -1)
		return true;

	auto CL_SendMove = []() {
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)utils::find_signature("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98");

		CL_SendMoveF();
	};

	auto WriteUsercmd = [](bf_write* buf, CUserCmd* in, CUserCmd* out) {
		using WriteUsercmd_t = void(__fastcall*)(bf_write*, CUserCmd*, CUserCmd*);
		static WriteUsercmd_t WriteUsercmdF = (WriteUsercmd_t)utils::find_signature("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");
		WriteUsercmdF(buf, in, out);

	};
	
	int* pNumBackupCommands = (int*)((uintptr_t)buf - 0x30);
	int* pNumNewCommands = (int*)((uintptr_t)buf - 0x2C);
	int32_t new_commands = *pNumNewCommands;
	
	auto nextcmdnumber = interfaces::clientstate->lastoutgoingcommand + interfaces::clientstate->chokedcommands + 1;
	auto totalnewcommands = min(game::main.m_shift, 16);
	game::main.m_shift -= totalnewcommands;

	from = -1;
	*pNumNewCommands = totalnewcommands;
	*pNumBackupCommands = 0;
	
	for (to = nextcmdnumber - new_commands + 1; to <= nextcmdnumber; to++)
	{
		if (!ofunct(ECX, nSlot, buf, from, to, true))
			return false;

		from = to;
	}

	auto lastrealcmd = interfaces::input->GetUserCmd(nSlot, from);

	CUserCmd fromcmd;

	if (lastrealcmd)
		fromcmd = *lastrealcmd;

	CUserCmd tocmd = fromcmd;
	tocmd.command_number = nextcmdnumber++;
	tocmd.tick_count += 100;

	for (int i = new_commands; i <= totalnewcommands; i++)
	{
		WriteUsercmd(buf, &tocmd, &fromcmd);
		fromcmd = tocmd;
		tocmd.command_number++;
		tocmd.tick_count++;
	}

	return true;
}