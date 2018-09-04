#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "memoryfonts.h"
#include "LagComp.h"
#include "Autowall.h"
#include "Hooks.h"

int width = 0;
int height = 0;
bool done = false;

namespace ColorAlpha
{
	VisualsStruct ESP_ctx;
	RECT bbox;
	float ESP_Fade[64];
}

visuals::visuals()
{
	//this is a value connected to variables leave it here
}

void visuals::OnPaintTraverse(C_BaseEntity* local)
	{
		CViewSetup* g_ViewSetup;

		CInput::CUserCmd* pCmd;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

		C_BaseEntity *pLocal = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());

		Worldmods(); // b1g Memory Leak
		Bullettracers();

		if (g_Options.Visuals.Active)
		{
			Nightmode();
		}

		if (g_Options.Visuals.Active && g_Options.Visuals.Hitmarker)
		{
			Hitmarker();
		}

		CCSGrenadeHint::instance().Paint();

		static class Text
		{
		public:
			std::string text;
			int side;
			int Font;
			Color color;

			Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
			{
			}
		};
		std::vector< Text > texts;

		if (local->IsAlive())
		{
			int W, H, cW, cH;
			Interfaces::g_Engine->GetScreenSize(W, H);
			cW = W / 2;
			cH = H / 2;

			if (g_Options.Visuals.Recoilcrosshair && g_Options.Visuals.Active)
			{
				Interfaces::g_Engine->GetScreenSize(width, height);
				if (local && local->IsAlive())
				{
					static auto cl_crosshair_recoil = Interfaces::g_CVar->FindVar("cl_crosshair_recoil");

					static Vector ViewAngles;

					Interfaces::g_Engine->GetViewAngles(ViewAngles);
					ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

					Vector fowardVec;
					AngleVectors(ViewAngles, &fowardVec);
					fowardVec *= 10000;

					Vector start = local->GetVecOrigin() + local->GetViewOffset();
					Vector end = start + fowardVec, endScreen;

					Color recoil_color(int(g_Options.Colors.color_recoil[0] * 255.f), int(g_Options.Colors.color_recoil[1] * 255.f), int(g_Options.Colors.color_recoil[2] * 255.f));
					if (g_Render->WorldToScreen(end, endScreen) && local->IsAlive())
					{
						switch (g_Options.Visuals.Recoilcrosshairtype)
						{
						case 0:
							cl_crosshair_recoil->SetValue(0);
							break;
						case 1:
							Interfaces::g_Engine->ClientCmd_Unrestricted("crosshair 0");
							g_Render->Line(endScreen.x - 4, endScreen.y, endScreen.x + 4, endScreen.y, recoil_color);
							g_Render->Line(endScreen.x, endScreen.y - 4, endScreen.x, endScreen.y + 4, recoil_color);
							cl_crosshair_recoil->SetValue(0);
							break;
						case 2:
							Interfaces::g_Engine->ClientCmd_Unrestricted("crosshair 0");
							g_Render->OutlineCircle(endScreen.x, endScreen.y, 3, 50, recoil_color);
							cl_crosshair_recoil->SetValue(0);
							break;
						case 3:
							Interfaces::g_Engine->ClientCmd_Unrestricted("crosshair 1");
							cl_crosshair_recoil->SetValue(1);
							break;

						}
					}
				}
			}

			if (g_Options.Visuals.Spreadcrosshair && g_Options.Visuals.Active)
			{
				RECT View = g_Render->GetViewport();
				int xs = View.right / 2;
				int ys = View.bottom / 2;

				if (local && local->IsAlive())
				{
					auto accuracy = pWeapon->GetInaccuracy() * 550.f; //3000

					ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.Spreadcrosshairalpha * 255.f);

					Color color(int(g_Options.Colors.color_spread[0] * 255.f), int(g_Options.Colors.color_spread[1] * 255.f), int(g_Options.Colors.color_spread[2] * 255.f), g_Options.Visuals.Spreadcrosshairalpha);

					g_Render->DrawFilledCircle2(xs, ys, accuracy, 100, color);
				}
			}

			if (g_Options.Visuals.Snipercrosshair && g_Options.Visuals.Active)
			{
				Color color(int(g_Options.Colors.color_sniper[0] * 255.f), int(g_Options.Colors.color_sniper[1] * 255.f), int(g_Options.Colors.color_sniper[2] * 255.f), 150);
				int damage;
				RECT View = g_Render->GetViewport();
				int pXs = View.right / 2;
				int pYs = View.bottom / 2;
				if (MiscFunctions::IsSniper(pWeapon))
				{
					switch (g_Options.Visuals.Snipercrosshairtype)
					{

					case 1:
						Interfaces::g_Surface->DrawSetColor(color);
						Interfaces::g_Surface->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
						Interfaces::g_Surface->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
						break;
					case 2:
						if (CanWallbang(damage))
						{
							Interfaces::g_Surface->DrawSetColor(0, 255, 0, 150);
							Interfaces::g_Surface->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
							Interfaces::g_Surface->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
						}
						else
						{
							Interfaces::g_Surface->DrawSetColor(255, 0, 0, 150);
							Interfaces::g_Surface->DrawFilledRect(cW - 6, cH - 1, cW - 3 + 9, cH - 1 + 2);
							Interfaces::g_Surface->DrawFilledRect(cW - 1, cH - 6, cW - 1 + 2, cH - 3 + 9);
						}
						break;
					case 3:
						g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, color);
						g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, color);
						break;
					case 4:
						if (CanWallbang(damage))
						{
							g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, Color(0, 255, 0));
							g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, Color(0, 255, 0));
						}
						else
						{
							g_Render->Line(pXs - 10, pYs, pXs + 10, pYs, Color(255, 0, 0));
							g_Render->Line(pXs, pYs - 10, pXs, pYs + 10, Color(255, 0, 0));
						}
						break;
					}

				}
			}
		}

		Color pColor;
		for (int i = 0; i < Interfaces::g_EntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity *entity = Interfaces::g_EntityList->GetClientEntity(i);

			if (entity == nullptr)
				continue;
			if (entity == local)
				continue;
			if (entity->IsDormant())
				continue;

			player_info_t pinfo;
			Vector max = entity->GetCollideable()->OBBMaxs();
			Vector pos, pos3D;
			pos3D = entity->GetVecOrigin();

			int owner = 0;
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			if (g_Options.Visuals.Droppedguns && g_Options.Visuals.Active && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
			{
				Color clr = Color(int(g_Options.Colors.droppedguns[0] * 255.f), int(g_Options.Colors.droppedguns[1] * 255.f), int(g_Options.Colors.droppedguns[2] * 255.f));

				RenderWeapon(entity, clr);
			}

			if (g_Options.Visuals.Bomb && g_Options.Visuals.Active)
			{
				if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
					DrawBombPlanted(entity, local);
			}

			if (cClass->m_ClassID == (int)ClassID::CC4)
			{
				DrawBomb(entity, cClass);
			}

			if (g_Options.Visuals.Grenades && g_Options.Visuals.GrenadeType && g_Options.Visuals.Active)
			{
				if (!g_Render->WorldToScreen(pos3D, pos))
					continue;

				Color GrenadeColor = Color(0, 0, 0, 0);
				char* szModelName = "Unknown Projectile";
				char* szModelIcon = "";
				if (strstr(cClass->m_pNetworkName, XorStr("Projectile")))
				{
					const model_t* pModel = entity->GetModel();
					if (!pModel)
						return;


					const studiohdr_t* pHdr = Interfaces::g_ModelInfo->GetStudiomodel(pModel);

					if (!pHdr)
						return;

					if (!strstr(pHdr->name, XorStr("thrown")) && !strstr(pHdr->name, XorStr("dropped")))
						return;


					IMaterial* mats[32];
					Interfaces::g_ModelInfo->GetModelMaterials(pModel, pHdr->numtextures, mats);

					for (int i = 0; i < pHdr->numtextures; i++)
					{
						IMaterial* mat = mats[i];
						if (!mat)
							continue;

						if (strstr(mat->GetName(), "flashbang"))
						{
							GrenadeColor = Color(255, 255, 0, 255);
							szModelName = "Flashbang";
							szModelIcon = "i";
						}
						else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
						{
							szModelName = "Grenade";
							szModelIcon = "j";
							GrenadeColor = Color(255, 0, 0, 255);
							break;
						}
						else if (strstr(mat->GetName(), "smoke"))
						{
							szModelName = "Smoke";
							szModelIcon = "k";
							GrenadeColor = Color(0, 255, 0, 255);
							break;
						}
						else if (strstr(mat->GetName(), "decoy"))
						{
							szModelName = "Decoy";
							szModelIcon = "m";
							GrenadeColor = Color(0, 255, 0, 255);
							break;
						}
						else if (strstr(mat->GetName(), "incendiary"))
						{
							szModelName = "Incendiary";
							szModelIcon = "n";
							GrenadeColor = Color(255, 0, 0, 255);
							break;
						}
						else if (strstr(mat->GetName(), "molotov"))
						{
							szModelName = "Molotov";
							szModelIcon = "l";
							GrenadeColor = Color(255, 0, 0, 255);
							break;
						}
					}
				}
			}

			if (Interfaces::g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
			{
				Color color(int(g_Options.Colors.backtrackdots_color[0] * 255.f), int(g_Options.Colors.backtrackdots_color[1] * 255.f), int(g_Options.Colors.backtrackdots_color[2] * 255.f));
				if (g_Options.Backtrack.backtrackenable)
				{
					if (local->IsAlive())
					{
						for (int t = 0; t < g_Options.Backtrack.backtrackticks; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
							{
								if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{
									if (g_Options.Backtrack.backtrackdots)
									{
										Interfaces::g_Surface->DrawSetColor(color);
										Interfaces::g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);
									}
								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				if (!g_Render->WorldToScreen(pos3D, pos))
					continue;

				Color clr = entity->GetTeamNum() == local->GetTeamNum() ?
					Color(g_Options.Colors.TeamESP[0] * 255, g_Options.Colors.TeamESP[1] * 255, g_Options.Colors.TeamESP[2] * 255, 255) :
					Color(g_Options.Colors.EnemyESP[0] * 255, g_Options.Colors.EnemyESP[1] * 255, g_Options.Colors.EnemyESP[2] * 255, 255);
				if (entity->GetTeamNum() == local->GetTeamNum() && !g_Options.Visuals.TeamESP)
					continue;
				if (!entity->IsAlive())
					continue;

				bool PVS = false;
				RECT rect = DynamicBox(entity, PVS, local);

				DrawInfo(rect, entity, local);

				if (g_Options.Visuals.Box && g_Options.Visuals.Active)
				{
					switch (g_Options.Visuals.Boxtype)
					{
					case 0:
						DrawBox(rect, clr);
						break;
					case 1:
						DrawCorners(rect, clr);
						break;
					case 2:
						ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetVecOrigin(), clr);
						break;
					}
				}

				if (g_Options.Visuals.FillESP && g_Options.Visuals.Active)
				{
					if (entity->GetTeamNum() == local->GetTeamNum())
					{
						Interfaces::g_Surface->DrawSetColor(Color(int(g_Options.Colors.fill_color_team[0] * 255.f), int(g_Options.Colors.fill_color_team[1] * 255.f), int(g_Options.Colors.fill_color_team[2] * 255.f), g_Options.Visuals.FillESPAmount));
						ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.FillESPAmount * 255.f);
						RenderFill(rect);
					}
					else if (entity->GetTeamNum() != local->GetTeamNum())
					{
						ColorAlpha::ESP_ctx.clr_fill.SetAlpha(g_Options.Visuals.FillESPAmount * 255.f);
						Interfaces::g_Surface->DrawSetColor(Color(int(g_Options.Colors.fill_color_enemy[0] * 255.f), int(g_Options.Colors.fill_color_enemy[1] * 255.f), int(g_Options.Colors.fill_color_enemy[2] * 255.f), g_Options.Visuals.FillESPAmount));

						RenderFill(rect);
					}
				}

				if (g_Options.Visuals.Snapline && g_Options.Visuals.Active)
				{
					DrawSnapLine(rect);
				}

				if (g_Options.Visuals.Skeleton && g_Options.Visuals.Active)
				{
					Skeleton(entity, Color(int(g_Options.Colors.color_skeleton[0] * 255.f), int(g_Options.Colors.color_skeleton[1] * 255.f), int(g_Options.Colors.color_skeleton[2] * 255.f)));

					if (g_Options.Visuals.Skeletonhealth)
					{
						HealthSkeleton(entity);
					}
				}

				if (g_Options.Visuals.Health && g_Options.Visuals.Active)
				{
					DrawHealth(rect, entity);
				}

				if (g_Options.Visuals.Armor && g_Options.Visuals.Active)
				{
					DrawArmorbar(rect, entity);
				}

				if (g_Options.Visuals.Aimdirection && g_Options.Visuals.Active)
				{
					DrawAimdirection(entity, Color(int(g_Options.Colors.BulletTraceColor[0] * 255.f), int(g_Options.Colors.BulletTraceColor[1] * 255.f), int(g_Options.Colors.BulletTraceColor[2] * 255.f)));
				}
			}
		}
	}

void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	Bomb = nullptr;

	float damage;
	char bombdamagestringdead[24];
	char bombdamagestringalive[24];

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (Interfaces::g_Globals->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a * exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "Health Left: 0");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health Left: %.0f", local->GetHealth() - damage);
			if (lifetime > -2.f)
			{
				if (damage >= local->GetHealth())
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.Defuse, bombdamagestringdead);
				}
				else if (local->GetHealth() > damage)
				{
					g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.Defuse, bombdamagestringalive);
				}
			}
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Planted C4: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.Defuse, buffer);
		}
	}

	Interfaces::g_Engine->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;

	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * Interfaces::g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.Defuse, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.Defuse, "NO TIME");
			}
		}
	}
}

void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	Bomb = nullptr;

	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	bool bomb = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (Interfaces::g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			Bomb = pParentEnt;
			bomb = false;
		}
	}

	if (g_Options.Visuals.Bomb)
	{
		if (bomb)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(45, 150, 103, 255), g_Render->font.ESP, "Dropped C4");
			}
		}
	}
}

void visuals::DrawArmorbar(RECT rect, C_BaseEntity* pEntity)
{
	float ArmorValue = pEntity->ArmorValue();
	int red = 255 - (ArmorValue * 2.0);
	int blue = ArmorValue * 2.0;
	float height = (rect.bottom - rect.top) * (ArmorValue / 100);

	g_Render->GradientH(rect.left - 11, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(0, 0, 0, 150), Color(0, 0, 0, 150));
	g_Render->GradientH(rect.left - 10, rect.bottom - height, 4, height, Color(89, 106, 135, 255), Color(89, 106, 135, 255));
	g_Render->DrawRect(rect.left - 11, rect.top - 1, 6, rect.bottom - rect.top + 2, Color(red, blue, blue, 255));
}

void visuals::DrawHealth(RECT rect, C_BaseEntity* pPlayer)
	{

		float top = 1.4;
		float right = 0;
		float left = 0;

		float HealthValue = pPlayer->GetHealth();
		int iHealthValue = HealthValue;
		int Red = 255 - (HealthValue * 2.00);
		int Green = HealthValue * 2.00;
		float flBoxes = std::ceil(pPlayer->GetHealth() / 10.f);

		float height = (rect.bottom - rect.top) * (HealthValue / 100);
		float height2 = (rect.bottom - rect.top) * (100 / 100);
		float flHeight = height2 / 10.f;


		g_Render->DrawRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, Color(0, 0, 0, 150));
		g_Render->DrawRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, Color(Red, Green, 0, 255));

		for (int i = 0; i < 10; i++)
			g_Render->Line(rect.left - 5, rect.top + i * flHeight, rect.left - 2, rect.top + i * flHeight, Color(0, 0, 0, 255));
}

void visuals::DrawBox(RECT rect, Color Col)
{
	g_Render->DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right - rect.left + 2, rect.bottom - rect.top + 2, Color(0, 0, 0, 150));
	g_Render->DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - rect.left - 2, rect.bottom - rect.top - 2, Color(0, 0, 0, 125));
	g_Render->DrawOutlinedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Col);
}

void visuals::RenderFill(RECT rect)
{
	Interfaces::g_Surface->DrawFilledRect(rect.left + 2, rect.top + 2, rect.right - 2, rect.bottom - 2);
}

void visuals::DrawCorners(RECT rect, Color Col)
{
	int x1, y1, x2, y2, w, h;
	x1 = rect.left;
	y1 = rect.top;
	x2 = rect.right;
	y2 = rect.bottom;

	w = x2;
	h = y2;

	int Line_Size = (y1 - h) / 6;
	int Line_Size2 = (y1 - h) / 6;

	int red = 0;
	int green = 0;
	int blue = 0;
	int alpha = 0;
	Col.GetColor(red, green, blue, alpha);
	Interfaces::g_Surface->DrawSetColor(red, green, blue, alpha);

	//top inwards
	Interfaces::g_Surface->DrawLine(w, y1, w + Line_Size, y1);
	Interfaces::g_Surface->DrawLine(x1, y1, x1 - Line_Size, y1);

	//top downwards
	Interfaces::g_Surface->DrawLine(x1, y1, x1, y1 - Line_Size);
	Interfaces::g_Surface->DrawLine(w, y1, w, y1 - Line_Size);

	//bottom inwards
	Interfaces::g_Surface->DrawLine(x1, h, x1 - Line_Size, h);
	Interfaces::g_Surface->DrawLine(w, h, w + Line_Size, h);

	//bottom upwards
	Interfaces::g_Surface->DrawLine(x1, h, x1, h + Line_Size);
	Interfaces::g_Surface->DrawLine(w, h, w, h + Line_Size);

	//outlines

	Interfaces::g_Surface->DrawSetColor(0, 0, 0, 200);

	//top inwards
	Interfaces::g_Surface->DrawLine(w, y1 - 1, w + Line_Size, y1 - 1);
	Interfaces::g_Surface->DrawLine(x1, y1 - 1, x1 - Line_Size, y1 - 1);
	//inlines
	Interfaces::g_Surface->DrawLine(w - 1, y1 + 1, w + Line_Size, y1 + 1);
	Interfaces::g_Surface->DrawLine(x1 + 1, y1 + 1, x1 - Line_Size, y1 + 1);

	// top downwards
	Interfaces::g_Surface->DrawLine(x1 - 1, y1 - 1, x1 - 1, y1 - Line_Size);
	Interfaces::g_Surface->DrawLine(w + 1, y1 - 1, w + 1, y1 - Line_Size);
	//inlines
	Interfaces::g_Surface->DrawLine(x1 + 1, y1, x1 + 1, y1 - Line_Size);
	Interfaces::g_Surface->DrawLine(w - 1, y1, w - 1, y1 - Line_Size);

	//bottom inwards
	Interfaces::g_Surface->DrawLine(x1, h + 1, x1 - Line_Size, h + 1);
	Interfaces::g_Surface->DrawLine(w, h + 1, w + Line_Size, h + 1);
	//inlines
	Interfaces::g_Surface->DrawLine(x1 + 1, h - 1, x1 - Line_Size, h - 1);
	Interfaces::g_Surface->DrawLine(w - 1, h - 1, w + Line_Size, h - 1);

	//bottom upwards
	Interfaces::g_Surface->DrawLine(x1 - 1, h + 1, x1 - 1, h + Line_Size);
	Interfaces::g_Surface->DrawLine(w + 1, h + 1, w + 1, h + Line_Size);
	//inlines
	Interfaces::g_Surface->DrawLine(x1 + 1, h, x1 + 1, h + Line_Size);
	Interfaces::g_Surface->DrawLine(w - 1, h, w - 1, h + Line_Size);
}

void visuals::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };

	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!g_Render->WorldToScreen(corners[edge[0]], p1) || !g_Render->WorldToScreen(corners[edge[1]], p2))
			return;
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		Col.GetColor(red, green, blue, alpha);
		Interfaces::g_Surface->DrawSetColor(red, green, blue, alpha);
		Interfaces::g_Surface->DrawLine(p1.x, p1.y, p2.x, p2.y);
	}
}

void visuals::DrawInfo(RECT rect, C_BaseEntity* entity, C_BaseEntity* local)
{
	player_info_t info;
	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;

	if (Interfaces::g_Engine->GetPlayerInfo(entity->GetIndex(), &info))
	{
		if (g_Options.Visuals.Name && g_Options.Visuals.Active)
		{
			texts.push_back(Text(info.name, 0, g_Render->font.ESP, Color(255, 255, 255, 255)));
		}

		if (g_Options.Visuals.Callout && g_Options.Visuals.Active)
		{
			texts.push_back(Text(entity->GetCallout(), 0, g_Render->font.ESP, Color(255, 220, 220, 255)));
		}

		if (g_Options.Visuals.Money && g_Options.Visuals.Active)
		{
			texts.push_back(Text(std::string("$") + std::to_string(entity->iAccount()), 1, g_Render->font.ESP, Color(124, 252, 0, 255)));
		}

		if (g_Options.Visuals.Armor2 && g_Options.Visuals.Active)
		{
			switch (g_Options.Visuals.Armor2)
			{
			case 0:
				break;
			case 1:
				texts.push_back(Text(entity->GetArmorName(), 1, g_Render->font.ESP, Color(255, 255, 255, 255)));
				break;
			case 2:
				texts.push_back(Text(entity->GetArmorIcon(), 1, g_Render->font.Icon, Color(255, 255, 255, 255)));
				break;
			default:
				break;
			}
		}

		if (g_Options.Visuals.Flags)
		{

			if (entity == HasBomb && g_Options.Visuals.Active)
			{
				texts.push_back(Text("Has C4", 1, g_Render->font.ESP, Color(255, 220, 220, 255)));
			}

			if (entity->IsFlashed() && g_Options.Visuals.Active)
			{
				texts.push_back(Text("Flashed", 0, g_Render->font.ESP, Color(255, 255, 183, 255)));
			}

			if (entity->IsScoped() && g_Options.Visuals.Active)
			{
				texts.push_back(Text("Scoped", 0, g_Render->font.ESP, Color(0, 202, 247, 255)));
			}

			if (entity->hasDefuser() && g_Options.Visuals.Active)
			{
				texts.push_back(Text("r", 1, g_Render->font.Icon, Color(255, 255, 183, 255))); //b1g defuser icon
			}

		}

		if (g_Options.Visuals.Distance && g_Options.Visuals.Active)
		{
			texts.push_back(Text(std::to_string(flGetDistance(local->GetVecOrigin(), entity->GetVecOrigin())) + std::string("M"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
		}

		if (g_Options.Visuals.Weapon && g_Options.Visuals.Active && g_Options.Visuals.WeaponType)
		{
			CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());
			if (weapon)
			{
				switch (g_Options.Visuals.WeaponType)
				{
				case 0:
					break;
				case 1:
					if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon))
					{
						texts.push_back(Text(weapon->GetGunName(), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
					}
					else
						texts.push_back(Text(weapon->GetGunName() + std::string("[") + std::to_string(weapon->ammo()) + std::string("|") + std::to_string(weapon->ammo2()) + std::string("]"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
					break;
				case 2:
					texts.push_back(Text(weapon->GetGunIcon(), 2, g_Render->font.Icon, Color(255, 255, 255, 255)));
					break;
				default:
					break;
				}
			}
		}
	}

	int middle = ((rect.right - rect.left) / 2) + rect.left;
	int Top[3] = { rect.top,rect.top, rect.bottom };
	for (auto text : texts)
	{
		RECT nameSize = g_Render->GetTextSize(text.Font, (char*)text.text.c_str());
		switch (text.side)
		{
		case 0:
			Top[0] -= nameSize.bottom + 1;
			g_Render->DrawString2(text.Font, middle, Top[0] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			break;
		case 1:
			g_Render->DrawString2(text.Font, rect.right + 2, Top[1] + 8, text.color, FONT_LEFT, (char*)text.text.c_str());
			Top[1] += nameSize.bottom - 3;
			break;
		case 2:
			g_Render->DrawString2(text.Font, middle, Top[2] + 8, text.color, FONT_CENTER, (char*)text.text.c_str());
			Top[2] += nameSize.bottom - 4;
			break;
		}
	}
}

RECT visuals::DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local)
{
	Vector trans = pPlayer->GetVecOrigin();

	Vector min;
	Vector max;

	min = pPlayer->GetCollideable()->OBBMins();
	max = pPlayer->GetCollideable()->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector Distance = pointList[0] - pointList[1];
	int dst = Distance.Length();
	dst /= 1.3f;
	Vector angs;
	CalcAngle(trans, local->GetEyePosition(), angs);

	Vector all[8];
	angs.y += 45;
	for (int i = 0; i < 4; i++)
	{
		AngleVectors(angs, &all[i]);
		all[i] *= dst;
		all[i + 4] = all[i];
		all[i].z = max.z;
		all[i + 4].z = min.z;
		VectorAdd(all[i], trans, all[i]);
		VectorAdd(all[i + 4], trans, all[i + 4]);
		angs.y += 90;
	}

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	PVS = true;

	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[3], flb))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[0], blb))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[2], frb))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[6], blt))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[5], brt))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[4], frt))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[1], brb))
		PVS = false;
	if (!Interfaces::g_DebugOverlay->ScreenPosition(all[7], flt))
		PVS = false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;

	return rect;
}

bool visuals::GetBBox(C_BaseEntity* entity, visuals::ESPBox &result)
{

	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;


	vOrigin = entity->GetAbsOrigin2();
	min = entity->collisionProperty()->GetMins() + vOrigin;
	max = entity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!g_Render->WorldToScreen(points[3], flb) || !g_Render->WorldToScreen(points[5], brt)
		|| !g_Render->WorldToScreen(points[0], blb) || !g_Render->WorldToScreen(points[4], frt)
		|| !g_Render->WorldToScreen(points[2], frb) || !g_Render->WorldToScreen(points[1], brb)
		|| !g_Render->WorldToScreen(points[6], blt) || !g_Render->WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;
	return true;
}

void visuals::RenderWeapon(C_BaseEntity* entity, Color color)
{
	if (entity)
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

		auto owner = pWeapon->GetOwnerHandle();

		if (owner > -1)
			return;

		Vector pos3D = entity->GetAbsOrigin2();

		if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
			return;

		Vector pos;

		if (!g_Render->WorldToScreen(pos3D, pos))
			return;

		int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
		auto weaponName = ItemDefinitionIndexToString(weaponID);
		auto weaponIcon = ItemDefinitionIndexToString2(weaponID);

		switch (g_Options.Visuals.Droppedgunstype)
		{
		case 0:
			break;
		case 1:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Guns, weaponName);
			break;
		case 2:
			g_Render->Text(pos.x, pos.y, color, g_Render->font.Icon, weaponIcon);
			break;
		default:
			break;
		}
	}
}

void visuals::DrawAimdirection(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * g_Options.Visuals.Aimdirectionlength);

	ray.Init(src3D, dst3D);

	Interfaces::g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);
};

void visuals::Skeleton(C_BaseEntity* pEntity, Color Col)
{
	studiohdr_t* pStudioHdr = Interfaces::g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && pBone->flags & BONE_USED_BY_HITBOX && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
			Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);

			Interfaces::g_Surface->DrawSetColor(Color(int(g_Options.Colors.color_skeleton[0] * 255.f), int(g_Options.Colors.color_skeleton[1] * 255.f), int(g_Options.Colors.color_skeleton[2] * 255.f)));
			Interfaces::g_Surface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
}

void visuals::HealthSkeleton(C_BaseEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = pEntity->GetBonePos(iChestBone + 1) - pEntity->GetBonePos(iChestBone); // direction vector from chest to neck
			vBreastBone = pEntity->GetBonePos(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			if (pEntity->GetHealth() == 100)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(0, 255, 0));
			}

			if (pEntity->GetHealth() < 100 & pEntity->GetHealth() > 85 || pEntity->GetHealth() == 85)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(114, 255, 00));

			}
			else if (pEntity->GetHealth() < 85 & pEntity->GetHealth() > 60 || pEntity->GetHealth() == 60)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(178, 255, 0));
			}
			else if (pEntity->GetHealth() < 60 & pEntity->GetHealth() > 45 || pEntity->GetHealth() == 45)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 229, 0));
			}
			else if (pEntity->GetHealth() < 45 & pEntity->GetHealth() > 30 || pEntity->GetHealth() == 30)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 127, 0));
			}
			else  if (pEntity->GetHealth() < 30 & pEntity->GetHealth() > 15 || pEntity->GetHealth() == 15)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 55, 0));
			}
			else  if (pEntity->GetHealth() < 15 & pEntity->GetHealth() > 0)
			{
				Interfaces::g_DebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces::g_DebugOverlay->ScreenPosition(vChild, sChild);
				g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(191, 0, 0));
			}
		}
	}
}

void visuals::DrawSnapLine(RECT rect)
{
	Color color = Color(int(g_Options.Colors.AimLineColor[0] * 255.f), int(g_Options.Colors.AimLineColor[1] * 255.f), int(g_Options.Colors.AimLineColor[2] * 255.f));

	int width, height;
	Interfaces::g_Engine->GetScreenSize(width, height);

	int screen_x = width * 0.5f,
		screen_y = height,
		target_x = rect.left + (rect.right - rect.left) * 0.5,
		target_y = rect.bottom,
		max_length = height * 0.3f;

	if (target_x == 0 ||
		target_y == 0)
		return;

	float length = sqrt(pow(target_x - screen_x, 2) + pow(target_y - screen_y, 2));
	if (length > max_length)
	{
		float
		x_normalized = (target_x - screen_x) / length,
		y_normalized = (target_y - screen_y) / length;
		target_x = screen_x + x_normalized * max_length;
		target_y = screen_y + y_normalized * max_length;
		g_Render->OutlineCircle(target_x + x_normalized * 3.5f, target_y + y_normalized * 3.5f, 8.f, 60, color);
	}

	Interfaces::g_Surface->DrawSetColor(color);
	Interfaces::g_Surface->DrawLine(screen_x, screen_y, target_x, target_y);
}

void visuals::Worldmods()
{
	static auto mat_postprocess_enable = Interfaces::g_CVar->FindVar("mat_postprocess_enable");

	if (g_Options.Visuals.Nopostprocess)
	{
		mat_postprocess_enable->SetValue(0);
	}
	else
	{
		mat_postprocess_enable->SetValue(1);
	}

	if (g_Options.Visuals.Ambientlight && g_Options.Visuals.Active)
	{
		float AmbientRedAmount = g_Options.Colors.ambientlightcolor[0];
		float AmbientGreenAmount = g_Options.Colors.ambientlightcolor[1];
		float AmbientBlueAmount = g_Options.Colors.ambientlightcolor[2];

		ConVar* AmbientRedCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_r");
		*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
		AmbientRedCvar->SetValue(AmbientRedAmount);

		ConVar* AmbientGreenCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_g");
		*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
		AmbientGreenCvar->SetValue(AmbientGreenAmount);

		ConVar* AmbientBlueCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_b");
		*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
		AmbientBlueCvar->SetValue(AmbientBlueAmount);
	}

	else if (!g_Options.Visuals.Ambientlight || !g_Options.Visuals.Active)
	{
		ConVar* AmbientRedCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_r");
		*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
		AmbientRedCvar->SetValue(0.f);

		ConVar* AmbientGreenCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_g");
		*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
		AmbientGreenCvar->SetValue(0.f);

		ConVar* AmbientBlueCvar = Interfaces::g_CVar->FindVar("mat_ambient_light_b");
		*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
		AmbientBlueCvar->SetValue(0.f);
	}
}

void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	Interfaces::g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 5, H / 2 - 5, W / 2 - 3, H / 2 - 3, Color(int(g_Options.Colors.hitmarker_color[0] * 255.f), int(g_Options.Colors.hitmarker_color[1] * 255.f), int(g_Options.Colors.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 5, H / 2 + 5, W / 2 - 3, H / 2 + 3, Color(int(g_Options.Colors.hitmarker_color[0] * 255.f), int(g_Options.Colors.hitmarker_color[1] * 255.f), int(g_Options.Colors.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 5, H / 2 - 5, W / 2 + 3, H / 2 - 3, Color(int(g_Options.Colors.hitmarker_color[0] * 255.f), int(g_Options.Colors.hitmarker_color[1] * 255.f), int(g_Options.Colors.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 5, H / 2 + 5, W / 2 + 3, H / 2 + 3, Color(int(g_Options.Colors.hitmarker_color[0] * 255.f), int(g_Options.Colors.hitmarker_color[1] * 255.f), int(g_Options.Colors.hitmarker_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
	}
}

void visuals::Nightmode()
{
	static auto sv_skyname = Interfaces::g_CVar->FindVar("sv_skyname");
	static auto static_prop = Interfaces::g_CVar->FindVar("r_DrawSpecificStaticProp");
	if (g_Options.Visuals.Nightmode && g_Options.Visuals.Active || !Interfaces::g_Engine->IsConnected() || !Interfaces::g_Engine->IsInGame())
	{
		if (!done)
		{
			sv_skyname->SetValue("sky_csgo_night02");
			static_prop->SetValue("0");

			for (MaterialHandle_t i = Interfaces::g_MaterialSystem->FirstMaterial(); i != Interfaces::g_MaterialSystem->InvalidMaterial(); i = Interfaces::g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = Interfaces::g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}
		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = Interfaces::g_MaterialSystem->FirstMaterial(); i != Interfaces::g_MaterialSystem->InvalidMaterial(); i = Interfaces::g_MaterialSystem->NextMaterial(i))
			{
				sv_skyname->SetValue("nukeblank");
				static_prop->SetValue("1");

				IMaterial *pMaterial = Interfaces::g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}
}

void visuals::Bullettracers()
{
	if (g_Options.Visuals.Bullettracers && g_Options.Visuals.Active && !Interfaces::g_Engine->IsConnected() && !Interfaces::g_Engine->IsInGame())
		return;

	auto local = static_cast<C_BaseEntity*>(Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer()));
	if (!local)
		return;

	for (size_t i = 0; i < logs.size(); i++)
	{
		auto current = logs.at(i);

		current.color = Color(g_Options.Colors.flTracers);

		if (g_Options.Visuals.Bullettracers && g_Options.Visuals.Active)
			Interfaces::g_DebugOverlay->AddLineOverlay(current.src, current.dst, current.color.r(), current.color.g(), current.color.b(), true, -1.f);

		Interfaces::g_DebugOverlay->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), current.color.r(), current.color.g(), current.color.b(), 127, -1.f);

		if (fabs(Interfaces::g_Globals->curtime - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}











































































































































































































































































































































































































































































































































































