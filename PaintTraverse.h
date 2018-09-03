#pragma once
#include "HookIncludes.h"
#include "Interfaces.h"
#include <ctime>
#include "ESP.h"
#include "Listener.h"

std::vector<const char*> vistasmoke_mats =
{
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};

typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);
bool once = true;
bool once1 = false;
int width1 = 0;
int height1 = 0;

void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	int w, h;
	int centerW, centerh, topH;
	Interfaces::g_Engine->GetScreenSize(w, h);
	centerW = w / 2;

	static auto ofunc = hooks::panel.get_original<paint_traverse_t>(41);
	C_BaseEntity* local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
	if (local != nullptr && local->IsAlive() && g_Options.Visuals.Noscopeborder && g_Options.Visuals.Active && !strcmp("HudZoom", Interfaces::g_Panel->GetName(vguiPanel)))
	{
		return;
	}
	ofunc(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;
	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::g_Panel->GetName(vguiPanel);
		if (strstr(szPanelName, "FocusOverlayPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Interfaces::g_Engine->IsConnected() && Interfaces::g_Engine->IsInGame())
		{
			Interfaces::g_Panel->SetMouseInputEnabled(vguiPanel, g_Options.Menu.Opened);
			static auto linegoesthrusmoke = U::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
			switch (g_Options.Visuals.Smoke)
			{
			case 0:
				for (auto mat_s : vistasmoke_mats)
				{
					IMaterial* mat = Interfaces::g_MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				break;
			case 1:
				for (auto mat_s : vistasmoke_mats)
				{
					IMaterial* mat = Interfaces::g_MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
				*(int*)(smokecout) = 0;
				break;
			case 2:
				*(int*)(smokecout) = 0;
				break;
			default:
				break;
			}
			Color color(int(g_Options.Colors.ScopeLine[0] * 255.f), int(g_Options.Colors.ScopeLine[1] * 255.f), int(g_Options.Colors.ScopeLine[2] * 255.f));
			visuals::instance().OnPaintTraverse(local);
			damage_indicators.paint();

			auto m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
			auto m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
			if (local != nullptr )
			{
				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
				if (g_Options.Visuals.Noflash)
				{
					*MakePtr(float*, local, m_flFlashDuration) = 0.f;
					*MakePtr(float*, local, m_flFlashMaxAlpha) = 0.f;
				}
				if (local && local->IsScoped() && g_Options.Visuals.Noscopeborder && MiscFunctions::IsSniper(pWeapon))
				{
					int width = 0;
					int height = 0;
					Interfaces::g_Engine->GetScreenSize(width, height);

					int centerX = static_cast<int>(width * 0.5f);
					int centerY = static_cast<int>(height * 0.5f);
					g_Render->Line(0, centerY, width, centerY, color);
					g_Render->Line(centerX, 0, centerX, height, color);
				}
			}
		}
		if (!Interfaces::g_Engine->IsInGame()) {
			g_Options.Misc.nameShitPaste = false;
			g_Options.Misc.spammeron = false;
		}
	}
}
