#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "LegitBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "ESP.h"
#include "OverrideView.h"
#include "LocalInfo.h"

using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	static auto ofunc = hooks::client.get_original<create_move_t>(22);
	ofunc(Interfaces::g_CHLClient, sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = Interfaces::g_Input->GetUserCmd(0, sequence_number);
	CInput::CUserCmd * pCmd;

	if (!cmd)
		return;

	static float next_time = 0;

	if (!cmd->command_number)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(Interfaces::g_Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

	if (!local)
		return;

	QAngle oldAngle = cmd->viewangles;
	QAngle oldangles;

	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	backtracking->legitBackTrack(cmd, local);

	if (g_Options.Misc.ServerRankRevealAll)
		RankReveal(cmd);

	switch (g_Options.Misc.AutoStrafe)
	{
	case 1:
		misc::LegitAutoStrafe(cmd, local, cmd->viewangles);
		break;
	case 2:
		misc::RageAutoStrafe(cmd, local, cmd->viewangles);
		break;
	}

	legitbot::instance().OnCreateMove(cmd, local, bSendPacket);
	misc::instance().OnCreateMove(cmd, local, bSendPacket);

	CCSGrenadeHint::instance().Tick(cmd->buttons);

	movementfix(oldAngle, cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();

	qLastTickAngles = cmd->viewangles;
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}