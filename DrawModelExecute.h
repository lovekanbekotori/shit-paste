#pragma once
#include "HookIncludes.h"
#include "LagComp.h"
#include <string>
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
using FindMaterialFn = IMaterial * (__thiscall*)(void*, char const*, const char*, bool, const char*);
float flColor[3];
float flVColor[3];
float flCColor[3];
float flCVColor[3];

IMaterial* GetCurrentMaterial(int mati)
{
	static IMaterial* mat;

	switch (mati)
	{
	case 3:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		break;
	case 4:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);
		break;
	case 5:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr);
		break;
	case 6:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr);
		break;
	case 7:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		break;
	default:
		mat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return mat;
		break;
	}
	return mat;
}

IMaterial* GetCurrentZMaterial(int mati)
{
	static IMaterial* zmat;

	switch (mati)
	{
	case 3:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.Chamsxqz);
		return zmat;
		break;
	case 4:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.Chamsxqz);
		return zmat;
		break;
	case 5:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.Chamsxqz);
		return zmat;
		break;
	case 6:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.Chamsxqz);
		return zmat;
		break;
	case 7:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", nullptr);
		zmat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, g_Options.Visuals.Chamsxqz);
		return zmat;
		break;
	default:
		zmat = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", nullptr);
		return zmat;
		break;
	}
}

/*IMaterial* __fastcall hkFindMaterial(void* thisptr, void*, const char* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
static auto oFindMaterial = hooks::modelrender.get_original<FindMaterialFn>(84);

if (strcmp(pMaterialName, "dev/scope_bluroverlay") == 0)
{
//return other material instead, hehe
static auto ClearAsShit = oFindMaterial(thisptr, "dev/clearalpha", nullptr, complain, pComplainPrefix);
return ClearAsShit; //I wonder if there's any slight performance gain/loss from this
}

return oFindMaterial(thisptr, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
} */

void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	static bool DontDraw = false;
	static auto ofunc = hooks::modelrender.get_original<dme_t>(21);

	static IMaterial* ignorez = CreateMaterial(true, true, false);
	static IMaterial* notignorez = CreateMaterial(false, true, false);

	IMaterial* zmat = GetCurrentZMaterial(g_Options.Visuals.Chamsstyle);
	IMaterial* mat = GetCurrentMaterial(g_Options.Visuals.Chamsstyle);

	IMaterial* hmat = GetCurrentMaterial(g_Options.Visuals.Chamsstyle);
	IMaterial* wmat = GetCurrentMaterial(g_Options.Visuals.Chamsstyle);

	static IMaterial* norm = CreateMaterial(false, true, false);
	static IMaterial* flat = CreateMaterial(false, false, false);
	static IMaterial* wire = CreateMaterial(false, false, true);

	static IMaterial* znorm = CreateMaterial(true, true, false);
	static IMaterial* zflat = CreateMaterial(true, false, false);
	static IMaterial* zwire = CreateMaterial(true, false, true);

	static IMaterial* Outline = CreateMaterial(true, false, true);
	DontDraw = false;

	const char * ModelName = Interfaces::g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
	C_BaseEntity* pModelEntity = (C_BaseEntity*)Interfaces::g_EntityList->GetClientEntity(pInfo.entity_index);
	C_BaseEntity* pLocal = (C_BaseEntity*)Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());

	int HandsStyle = g_Options.Visuals.Handstyle;
	int Chamsweps = g_Options.Visuals.Weaponstyle;

	if (ModelName && strstr(ModelName, "models/player"))
	{
		if (pModelEntity && pLocal)
		{
			if ((g_Options.Visuals.Chamsteam || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()) && g_Options.Visuals.Chamsplayer && g_Options.Visuals.Active)
			{
				pModelEntity = Interfaces::g_EntityList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Colors.EnemyChamsNVis[0];
					flColor[1] = g_Options.Colors.EnemyChamsNVis[1];
					flColor[2] = g_Options.Colors.EnemyChamsNVis[2];
					flVColor[0] = g_Options.Colors.EnemyChamsVis[0];
					flVColor[1] = g_Options.Colors.EnemyChamsVis[1];
					flVColor[2] = g_Options.Colors.EnemyChamsVis[2];
				}
				if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
				{
					flColor[0] = g_Options.Colors.TeamChamsNVis[0];
					flColor[1] = g_Options.Colors.TeamChamsNVis[1];
					flColor[2] = g_Options.Colors.TeamChamsNVis[2];
					flVColor[0] = g_Options.Colors.TeamChamsVis[0];
					flVColor[1] = g_Options.Colors.TeamChamsVis[1];
					flVColor[2] = g_Options.Colors.TeamChamsVis[2];
				}
				if (pModelEntity)
				{
					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
					{
						if (g_Options.Visuals.Chamsstyle == 0)
						{
							if (g_Options.Visuals.Chamsxqz)
							{
								Interfaces::g_RenderView->SetColorModulation(flColor);
								Interfaces::g_ModelRender->ForcedMaterialOverride(znorm);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::g_RenderView->SetColorModulation(flVColor);
							Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.Chamsstyle == 1)
						{
							if (g_Options.Visuals.Chamsxqz)
							{
								Interfaces::g_RenderView->SetColorModulation(flColor);
								Interfaces::g_ModelRender->ForcedMaterialOverride(zflat);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::g_RenderView->SetColorModulation(flVColor);
							Interfaces::g_ModelRender->ForcedMaterialOverride(flat);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (g_Options.Visuals.Chamsstyle == 2)
						{
							if (g_Options.Visuals.Chamsxqz)
							{
								Interfaces::g_RenderView->SetColorModulation(flColor);
								Interfaces::g_ModelRender->ForcedMaterialOverride(zwire);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::g_RenderView->SetColorModulation(flVColor);
							Interfaces::g_ModelRender->ForcedMaterialOverride(wire);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else
						{
							if (g_Options.Visuals.Chamsxqz)
							{
								Interfaces::g_RenderView->SetColorModulation(flColor);
								Interfaces::g_ModelRender->ForcedMaterialOverride(zmat);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							Interfaces::g_RenderView->SetColorModulation(flVColor);
							Interfaces::g_ModelRender->ForcedMaterialOverride(mat);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
					}
				}
				if (g_Options.Visuals.Chamshealth && g_Options.Visuals.Chamsplayer && g_Options.Visuals.Active)
				{
					float color1[3] = { 0.f, 1.f, 0.f };
					float color2[3] = { 1.f, 1.f, 0.f };
					float color3[3] = { 1.f, 0.f, 0.f };

					float alpha = 1.f;
					if (pModelEntity->HasGunGameImmunity())
						alpha = 0, 1.f;

					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && pModelEntity->GetTeamNum() != pLocal->GetTeamNum() && pModelEntity != pLocal)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 50)
						{
							Interfaces::g_RenderView->SetColorModulation(color1);
							Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 50 && pModelEntity->GetHealth() > 25)
						{
							Interfaces::g_RenderView->SetColorModulation(color2);
							Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 25)
						{
							Interfaces::g_RenderView->SetColorModulation(color3);
							Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
							ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
					}
					else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && g_Options.Visuals.Chamsteam && pModelEntity != pLocal)
					{
						if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
						{
							if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 50)
							{
								Interfaces::g_RenderView->SetColorModulation(color1);
								Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 50 && pModelEntity->GetHealth() > 25)
							{
								Interfaces::g_RenderView->SetColorModulation(color2);
								Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else if (pModelEntity->IsAlive() && pModelEntity->GetHealth() < 25)
							{
								Interfaces::g_RenderView->SetColorModulation(color3);
								Interfaces::g_ModelRender->ForcedMaterialOverride(norm);
								ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
						}
					}
				}
			}
		}
	}

	else if (HandsStyle != 0 && g_Options.Visuals.Active && strstr(ModelName, "arms"))
	{
		if (HandsStyle == 1)
		{
			DontDraw = true;
		}

		if (HandsStyle == 2)
		{
			Interfaces::g_ModelRender->ForcedMaterialOverride(notignorez);
			Interfaces::g_RenderView->SetColorModulation(g_Options.Colors.HandsColor);
		}

		if (HandsStyle == 3)
		{
			IMaterial* WireFrameHands = Interfaces::g_MaterialSystem->FindMaterial(ModelName, "Model textures");
			WireFrameHands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			Interfaces::g_ModelRender->ForcedMaterialOverride(WireFrameHands);
			Interfaces::g_RenderView->SetColorModulation(g_Options.Colors.HandsColor);
		}

		if (HandsStyle == 4)
		{
			IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", "Model textures");
			Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
		}

		if (HandsStyle == 5)
		{
			IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", "Model textures");
			Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
		}

		if (HandsStyle == 6)
		{
			IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", "Model textures");
			Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
		}

		if (HandsStyle == 7)
		{
			IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", "Model textures");
			Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
		}

		if (HandsStyle == 8)
		{
			IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", "Model textures");
			Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
		}
	}

	else if (Chamsweps != 0 && g_Options.Visuals.Active && strstr(ModelName, "weapons/v"))
	{
		if (!strstr(ModelName, "arms"))
		{
			if (Chamsweps == 1)
			{
				Interfaces::g_ModelRender->ForcedMaterialOverride(notignorez);
				Interfaces::g_RenderView->SetColorModulation(g_Options.Colors.WeaponColor);
			}

			if (Chamsweps == 2)
			{
				IMaterial* WireFrameWeps = Interfaces::g_MaterialSystem->FindMaterial(ModelName, "Model textures");
				WireFrameWeps->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces::g_ModelRender->ForcedMaterialOverride(WireFrameWeps);
				Interfaces::g_RenderView->SetColorModulation(g_Options.Colors.WeaponColor);
			}

			if (Chamsweps == 3)
			{
				IMaterial* Weapon = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", "Model textures");
				Interfaces::g_ModelRender->ForcedMaterialOverride(Weapon);
			}

			if (Chamsweps == 4)
			{
				IMaterial* Weapon = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", "Model textures");
				Interfaces::g_ModelRender->ForcedMaterialOverride(Weapon);
			}

			if (Chamsweps == 5)
			{
				IMaterial* Weapon = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gloss", "Model textures");
				Interfaces::g_ModelRender->ForcedMaterialOverride(Weapon);
			}

			if (Chamsweps == 6)
			{
				IMaterial* Hands = Interfaces::g_MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", "Model textures");
				Interfaces::g_ModelRender->ForcedMaterialOverride(Hands);
			}

			if (Chamsweps == 7)
			{
				IMaterial* Weapon = Interfaces::g_MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", "Model textures");
				Interfaces::g_ModelRender->ForcedMaterialOverride(Weapon);
			}
		}
	}
	if (!DontDraw)
	{
		ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		Interfaces::g_ModelRender->ForcedMaterialOverride(NULL);
	}
}