#include "Misc.h"
#include "Interfaces.h"
#include "Render.h"
#include <time.h>
#include "BaseClient.h"
#include "Strafer.h"
#include <chrono>

void SetClantag2(const char *tag)
{
	static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))U::pattern_scan(GetModuleHandle("engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

	fnClantagChanged(tag, tag);
}

void marquee(std::string& panicova_zlomena_noha)
{
	std::string temp_string = panicova_zlomena_noha;
	panicova_zlomena_noha.erase(0, 1);
	panicova_zlomena_noha += temp_string[0];
}

std::string animatedclantag;
int iLastTime;
bool bDone = false;
void misc::OnCreateMove(CInput::CUserCmd *cmd, C_BaseEntity *local, bool& bSendPacket)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	C_BaseEntity* pLocal = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());

	if (g_Options.Misc.Bhop && local->IsAlive())
	{
		if (cmd->buttons & IN_JUMP && !(local->GetMoveType() & MOVETYPE_LADDER))
		{

			int iFlags = local->GetFlags();
			if (!(iFlags & FL_ONGROUND))
				cmd->buttons &= ~IN_JUMP;
		}
	}
	if (g_Options.Misc.FakeLag)
	{
		{
			int iChoke = g_Options.Misc.FakeLagSlider;
			static int Packets = 0;
			if (Packets < iChoke) {
				bSendPacket = false; //real here
				if (g_Options.LegitAA.legitaaenable == true && g_Options.LegitAA.legitaatype > 0)
				{
					if (!(cmd->buttons & IN_USE) || !local->GetMoveType() == MOVETYPE_LADDER || !local->GetMoveType() == MOVETYPE_NOCLIP || !cmd->buttons & IN_ATTACK || !cmd->buttons & IN_ATTACK2 || !MiscFunctions::IsGrenade(pWeapon) || !MiscFunctions::IsKnife(pWeapon))
					{
						switch (g_Options.LegitAA.legitaatype)
						{
						case 1:
							cmd->viewangles.y += 180.f;
							break;
						case 2:
							cmd->viewangles.y += 90.f;
							break;
						case 3:
							static bool dir = false;
							static bool dir2 = false;
							//int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i;
							//IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
							//if (pCmd->forwardmove > 1 || (IsVisible(pLocal, pEntity, 0) && pEntity->GetTeamNum() != pLocal->GetTeamNum()))// was trying to make a vis check to make it -180 if their visible //didnt seem to work
							//dir2 = true;
							//else {
							dir2 = false;
							if (cmd->sidemove > 1) dir = true;
							else if (cmd->sidemove < -1) dir = false;
							cmd->viewangles.y = (dir) ? (cmd->viewangles.y - 180) - 270.f : (cmd->viewangles.y - 180) - 90.f;
							break;
						}
					}
				}
				else
					cmd->viewangles.y += 0;
			}
			if (Packets > iChoke) {
				Packets = 0;
				bSendPacket = true; //fake here
			}
			Packets++;
		}
	}
	if (g_Options.Misc.syncclantag)
	{
			static std::string cur_clantag = "                        shitpaste.cc";
			static int old_time;

			static int i = 0;
			if (Interfaces::g_Engine->IsInGame())
			{
				if (i > 16)
				{
					marquee(cur_clantag);
					SetClantag2(cur_clantag.c_str());
					i = 0;
				}
				else
				{
					i++;
				}
			}

	}
	if (g_Options.Misc.nameshitpaste)
	{
		char name[151];
		char fucked_char = static_cast<char>(-1);
		for (auto i = 0; i <= 150; i++)
			name[i] = fucked_char;

		const char nick[12] = "deacell.cc";
		memcpy(name, nick, 11);

		SetName(name);
	}
	// Auto Pistol
	static bool WasFiring = false;

	if (pWeapon != nullptr)
	{
		if (MiscFunctions::IsPistol(pWeapon) && g_Options.LegitBot.AutoPistol && pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex() != 64)
		{
			if (cmd->buttons & IN_ATTACK && !MiscFunctions::IsKnife(pWeapon) && !MiscFunctions::IsGrenade(pWeapon))
			{
				if (WasFiring)
				{
					cmd->buttons &= ~IN_ATTACK;
				}
			}

			WasFiring = cmd->buttons & IN_ATTACK ? true : false;
		}
	}
	ChatSpammer();


	if (g_Options.Misc.eventlogs2)
		logsrun2();

}

void misc::logsrun()
{

	static auto developer = Interfaces::g_CVar->FindVar("developer");
	developer->SetValue(1);
	static auto con_filter_text = Interfaces::g_CVar->FindVar("con_filter_text");
	static auto con_filter_text_out = Interfaces::g_CVar->FindVar("con_filter_text_out");
	static auto con_filter_enable = Interfaces::g_CVar->FindVar("con_filter_enable");

	con_filter_text->SetValue("hit ");
	con_filter_text_out->SetValue("");
	con_filter_enable->SetValue(2);
}

void misc::logsrun2()
{

	static auto developer = Interfaces::g_CVar->FindVar("developer");
	developer->SetValue(1);
	static auto con_filter_text = Interfaces::g_CVar->FindVar("con_filter_text");
	static auto con_filter_text_out = Interfaces::g_CVar->FindVar("con_filter_text_out");
	static auto con_filter_enable = Interfaces::g_CVar->FindVar("con_filter_enable");

	con_filter_text->SetValue("bought ");
	con_filter_text_out->SetValue("");
	con_filter_enable->SetValue(2);
}


std::vector<std::string> msgs =
{
	"shitpaste.cc b1g hak"
};

std::vector<std::string> msgs2 =
{
	""
};
void misc::ChatSpammer()
{
	static DWORD lastspammed = 0;

	if (!Interfaces::g_Engine->IsInGame() || !Interfaces::g_Engine->IsConnected() || !g_Options.Misc.spammeron)
		return;



	switch (g_Options.Misc.spammer)
	{

	case 0: break;

	case 1:
		if (GetTickCount() - lastspammed > 800)
		{
			lastspammed = GetTickCount();
			auto say = "say ";
			std::string p = say + msgs[rand() % msgs.size()];
			Interfaces::g_Engine->ClientCmd_Unrestricted(p.c_str());
		}
		break;

	case 2:
		if (GetTickCount() - lastspammed > 800)
		{
			lastspammed = GetTickCount();
			auto say = "say ";
			std::string p = say + msgs2[rand() % msgs2.size()];
			Interfaces::g_Engine->ClientCmd_Unrestricted(p.c_str());
		}
		break;
	}
}

void misc::silentname(C_BaseEntity *local)
{
	bool bDidMeme = false;
	int iNameIndex = -1;
	char chName[130];
	static ConVar* name = Interfaces::g_CVar->FindVar("name");
	{
		for (int iPlayerIndex = 0; iPlayerIndex < Interfaces::g_Engine->GetMaxClients(); iPlayerIndex++)
		{
			C_BaseEntity *pEntity = Interfaces::g_EntityList->GetClientEntity(iPlayerIndex);
			if (!pEntity || pEntity == local || iPlayerIndex == Interfaces::g_Engine->GetLocalPlayer())
				continue;
			if (rand() % 3 == 1)
			{
				iNameIndex = pEntity->GetIndex();
				bDidMeme = true;
			}
		}
		if (bDidMeme)
		{
			player_info_t pInfo;
			Interfaces::g_Engine->GetPlayerInfo(iNameIndex, &pInfo);
			sprintf(chName, "%s ", pInfo.name);
			name->SetValue(chName);
		}
	}
}

static vec_t Normalize_y(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;

}

void misc::LegitAutoStrafe(CInput::CUserCmd *cmd, C_BaseEntity *local, QAngle oldangles)
{
	static AutoStrafer Strafer;

	static float move = 450;
	float s_move = move * 0.5065f;
	if (local->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
		return;
	if (cmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
		return;

	if (cmd->buttons & IN_JUMP | !(local->GetFlags() & FL_ONGROUND))
	{
		if (local->GetVelocity().Length2D() == 0 && (cmd->forwardmove == 0 && cmd->sidemove == 0))
		{
			cmd->forwardmove = 450.f;
		}
		else if (cmd->forwardmove == 0 && cmd->sidemove == 0)
		{
			if (cmd->mousedx > 0 || cmd->mousedx > -0)
			{
				cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
			}
			else
			{
				auto airaccel = Interfaces::g_CVar->FindVar("sv_airaccelerate");
				auto maxspeed = Interfaces::g_CVar->FindVar("sv_maxspeed");

				static int zhop = 0;
				double yawrad = Normalize_y(oldangles.y) * PI / 180;

				float speed = maxspeed->GetFloat();
				if (cmd->buttons & IN_DUCK)
					speed *= 0.333;

				double tau = Interfaces::g_Globals->interval_per_tick, MA = speed * airaccel->GetFloat();

				int Sdir = 0, Fdir = 0;
				Vector velocity = local->GetVelocity();
				double vel[3] = { velocity[0], velocity[1], velocity[2] };
				double pos[2] = { 0, 0 };
				double dir[2] = { std::cos((oldangles[1] + 10 * zhop) * PI / 180), std::sin((oldangles[1] + 10 * zhop) * PI / 180) };
				oldangles.y = Normalize_y(yawrad * 180 / PI);
				Strafer.strafe_line_opt(yawrad, Sdir, Fdir, vel, pos, 30.0, tau, MA, pos, dir);
				cmd->sidemove = Sdir * 450;
			}
		}

	}
	movementfix(oldangles, cmd);
}

void misc::RageAutoStrafe(CInput::CUserCmd *cmd, C_BaseEntity* local, QAngle oldangles)
{
	if (g_Options.Misc.AutoStrafe)
	{
		static AutoStrafer Strafer;

		if (local->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
			return;
		if (cmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
			return;

		if (cmd->buttons & IN_JUMP | !(local->GetFlags() & FL_ONGROUND))
		{
			if (cmd->mousedx > 1 || cmd->mousedx < -1)
				cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
			else
			{
				cmd->forwardmove = (1800.f * 4.f) / local->GetVelocity().Length2D();
				cmd->sidemove = (cmd->command_number % 2 == 0 ? -450.f : 450.f);
			}
		}
		movementfix(oldangles, cmd);
	}
}





























































































































































































































































































































































































































































































































































