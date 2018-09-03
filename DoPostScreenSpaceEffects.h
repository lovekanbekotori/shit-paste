#pragma once
#include "HookIncludes.h"
using do_post_screen_space_effects_t = bool(__thiscall*)(void*, CViewSetup*);


bool _fastcall hkDoPostScreenSpaceEffects(void* ecx, void* edx, CViewSetup* pSetup)
{
	static auto ofunc = hooks::clientmode.get_original<do_post_screen_space_effects_t>(44);

	IMaterial *pMatGlowColor = Interfaces::g_MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	Interfaces::g_ModelRender->ForcedMaterialOverride(pMatGlowColor);

	if (g_Options.Visuals.Active && Interfaces::g_GlowObjManager && Interfaces::g_Engine->IsConnected())
	{
		auto local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
		if (local)
		{
			for (int i = 0; i < Interfaces::g_GlowObjManager->m_GlowObjectDefinitions.Count(); ++i)
			{
				if (Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
					continue;

				CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &Interfaces::g_GlowObjManager->m_GlowObjectDefinitions[i];

				switch (glowEnt->getEnt()->GetClientClass()->m_ClassID)
				{
				default:
					if (g_Options.Visuals.Glowweapon && g_Options.Visuals.Active)
					{
						if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")))
							glowEnt->set(g_Options.Colors.glow_weapon[0], g_Options.Colors.glow_weapon[1], g_Options.Colors.glow_weapon[2], g_Options.Visuals.Glowweaponalpha / 255.f);
					}
					break;
				case 1:
				case 39:
					if (g_Options.Visuals.Glowweapon && g_Options.Visuals.Active)
					{
						glowEnt->set(g_Options.Colors.glow_weapon[0], g_Options.Colors.glow_weapon[1], g_Options.Colors.glow_weapon[2], g_Options.Visuals.Glowweaponalpha / 255.f);
					}
					break;
				case 35:

				{
					if (g_Options.Visuals.Glowteam && glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
						glowEnt->set(g_Options.Colors.TeamGlow[0], g_Options.Colors.TeamGlow[1], g_Options.Colors.TeamGlow[2], g_Options.Visuals.Glowteamalpha / 255.f);
					else if (g_Options.Visuals.Glowplayer && glowEnt->getEnt()->GetTeamNum() != local->GetTeamNum())
						glowEnt->set(g_Options.Colors.EnemyGlow[0], g_Options.Colors.EnemyGlow[1], g_Options.Colors.EnemyGlow[2], g_Options.Visuals.Glowplayeralpha / 255.f);
				}
				break;

				case 29:
				case 105:
					if (g_Options.Visuals.Glowbomb && g_Options.Visuals.Active)
					{
						glowEnt->set(g_Options.Colors.glow_c4[0], g_Options.Colors.glow_c4[1], g_Options.Colors.glow_c4[2], g_Options.Visuals.Glowbombalpha / 255.f);
					}
					break;
				}
			}
		}
	}
	return ofunc(ecx, pSetup);
}