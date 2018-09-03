#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include <cctype>
#include "Listener.h"
#include "SpecList.h"
#include "namespammer.h"
#include "Radar.h"
#include "memoryfonts.h"
#include "SkinParser.h"
#include "SkinFilter.h"
#include <vector>

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define BUILDSTAMP ( __DATE__ )

void FullUpdate()
{
	Interfaces::g_Engine->ClientCmd_Unrestricted("cl_fullupdate 1");
}

//========Clantag====================

void SetClantag(const char *tag)
{
	static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))U::pattern_scan(GetModuleHandle("engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

	fnClantagChanged(tag, tag);
}
//--------------------------------------------
//Weapon CFG stuff

const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";
	default:
		return "";
	}
}

bool IsUtility(ItemDefinitionIndexx index)
{
	switch (index)
	{
	case ItemDefinitionIndexx::ITEM_NONE:
	case ItemDefinitionIndexx::WEAPON_C4:
	case ItemDefinitionIndexx::WEAPON_FLASH:
	case ItemDefinitionIndexx::WEAPON_HE:
	case ItemDefinitionIndexx::WEAPON_INC:
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
	case ItemDefinitionIndexx::WEAPON_SMOKE:
	case ItemDefinitionIndexx::WEAPON_DECOY:
	case ItemDefinitionIndexx::WEAPON_TASER:
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
	case ItemDefinitionIndexx::WEAPON_KNIFE_CT:
	case ItemDefinitionIndexx::GLOVE_T_SIDE:
	case ItemDefinitionIndexx::GLOVE_CT_SIDE:
	case ItemDefinitionIndexx::GLOVE_SPORTY:
	case ItemDefinitionIndexx::GLOVE_SLICK:
	case ItemDefinitionIndexx::GLOVE_LEATHER_WRAP:
	case ItemDefinitionIndexx::GLOVE_STUDDED_BLOODHOUND:
	case ItemDefinitionIndexx::GLOVE_MOTORCYCLE:
	case ItemDefinitionIndexx::GLOVE_SPECIALIST:
	case ItemDefinitionIndexx::GLOVE_HYDRA:
		return true;
	default:
		return false;
	}
}

bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

	return str;
}

bool defaultk;
bool bayonet;
bool flip;
bool gut;
bool karambit;
bool m9bayonet;
bool huntsman;
bool falchion;
bool bowie;
bool butterfly;
bool daggers;

void doknives(bool* disknife)
{
	defaultk = false;
	bayonet = false;
	flip = false;
	gut = false;
	karambit = false;
	m9bayonet = false;
	huntsman = false;
	falchion = false;
	bowie = false;
	butterfly = false;
	daggers = false;

	*disknife = true;
}

void bordernew()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Border] = ImColor(83, 85, 89, 255);
}

int ParseSkins()
{
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	if (!r)
		return 1;

	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 2;

	auto weaponSkinCombo = doc.BreadthFirstSearch("weapon_icons");
	if (!weaponSkinCombo || !weaponSkinCombo->ToObject())
		return 3;

	auto skinDataVec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (!skinDataVec.size())
		return 4;

	auto PaintKitNames = english.BreadthFirstSearch("Tokens");
	if (!PaintKitNames || !PaintKitNames->ToObject())
		return 5;

	//std::unordered_map<std::string, std::set<std::string>> G::weaponSkins;
	//std::unordered_map<std::string, skinInfo> G::skinMap;
	//std::unordered_map<std::string, std::string> G::skinNames;

	std::vector<std::string> weaponNames = {
		"deagle",
		"elite",
		"fiveseven",
		"glock",
		"ak47",
		"aug",
		"awp",
		"famas",
		"g3sg1",
		"galilar",
		"m249",
		"m4a1_silencer", //needs to be before m4a1 else silencer doesnt get filtered out :D
		"m4a1",
		"mac10",
		"p90",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"tec9",
		"hkp2000",
		"mp7",
		"mp9",
		"nova",
		"p250",
		"scar20",
		"sg556",
		"ssg08",
		"usp_silencer",
		"cz75a",
		"revolver",
		"knife_m9_bayonet", //needs to be before bayonet else knife_m9_bayonet doesnt get filtered out :D
		"bayonet",
		"knife_flip",
		"knife_gut",
		"knife_karambit",
		"knife_tactical",
		"knife_falchion",
		"knife_survival_bowie",
		"knife_butterfly",
		"knife_push",
		"studded_bloodhound_gloves",
		"sporty_gloves",
		"slick_gloves",
		"leather_handwraps",
		"motorcycle_gloves",
		"specialist_gloves"
	};

	//populate G::weaponSkins
	for (auto child : weaponSkinCombo->children)
	{
		if (child->ToObject())
		{
			for (auto weapon : weaponNames)
			{
				auto skinName = child->ToObject()->GetKeyByName("icon_path")->ToKeyValue()->Value.toString();
				auto pos = skinName.find(weapon);
				//filter out the skinname
				if (pos != std::string::npos)
				{
					auto pos2 = skinName.find_last_of('_');
					g_Options.Skinchanger.weaponSkins[weapon].insert(
						skinName.substr(pos + weapon.length() + 1,
							pos2 - pos - weapon.length() - 1)
					);
					break;
				}
			}
		}
	}

	//populate skinData
	for (auto skinData : skinDataVec)
	{
		if (skinData->ToObject())
		{
			for (auto skin : skinData->children)
			{
				if (skin->ToObject())
				{
					skinInfo si;
					si.paintkit = skin->ToObject()->name.toInt();

					auto skinName = skin->ToObject()->GetKeyByName("name")->ToKeyValue()->Value.toString();
					auto tagNode = skin->ToObject()->GetKeyByName("description_tag");
					if (tagNode)
					{
						std::string tag = tagNode->ToKeyValue()->Value.toString();
						tag = tag.substr(1, std::string::npos); //skip #
						std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
						si.tagName = tag;
					}

					auto keyVal = skin->ToObject()->GetKeyByName("seed");
					if (keyVal != nullptr)
						si.seed = keyVal->ToKeyValue()->Value.toInt();

					g_Options.Skinchanger.skinMap[skinName] = si;
				}
			}
		}
	}

	//populate G::skinNames
	for (auto child : PaintKitNames->children)
	{
		if (child->ToKeyValue())
		{
			std::string key = child->ToKeyValue()->Key.toString();
			std::transform(key.begin(), key.end(), key.begin(), towlower);
			if (key.find("paintkit") != std::string::npos &&
				key.find("tag") != std::string::npos)
			{
				g_Options.Skinchanger.skinNames[key] = child->ToKeyValue()->Value.toString();
			}
		}
	}

	return 0;
}

void initializeskins()
{
	static bool once = false;

	if (!once)
	{
		ParseSkins();
		once = true;
	}
}

namespace ShitPaste
{
	ImFont* fDefault;
	ImFont* fkek2;
	ImFont* ftabs;

	void GUI_Init(HWND window, IDirect3DDevice9 *pDevice)
	{

		if (ImGui_ImplDX9_Init(window, pDevice))
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			fDefault = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VERDANA.ttf", 13.0f);
			fkek2 = io.Fonts->AddFontFromMemoryCompressedTTF(myfont3_compressed_data, myfont3_compressed_size, 22.f);
			ftabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\IMPACT.ttf", 32.0f);

			auto& style = ImGui::GetStyle();

			style.Alpha = 1.0f;
			G::extra_flags = 0;
			style.WindowPadding = ImVec2(15, 15);
			style.WindowRounding = 5.0f;
			style.FramePadding = ImVec2(3, 3);
			style.FrameRounding = 0.0f;
			style.ItemSpacing = ImVec2(12, 8);
			style.ItemInnerSpacing = ImVec2(8, 6);
			style.IndentSpacing = 25.0f;
			style.ScrollbarSize = 15.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabMinSize = 5.0f;
			style.GrabRounding = 3.0f;

			style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_ChildWindowBg] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_PopupBg] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
			style.Colors[ImGuiCol_TitleBgActive] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImColor(142, 32, 32, 255);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(13, 13, 16, 255);
			style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
			style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

			G::d3dinit = true;
		}
	}

	void mainWindow()
	{
		C_BaseEntity* local;

		auto& style = ImGui::GetStyle();
		static int page = 1;
		static int weapontab = 1;

		ImGui::SetNextWindowSize(ImVec2(970.f, 530.f));

		if (ImGui::Begin("##main", &g_Options.Menu.Opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar));
		{
			static float r = 1.0f;
			static float g = 0.f;
			static float b = 0.f;

			if (r == 1.f && g >= 0.f && b <= 0.f)
			{
				g += 0.0005f;
				b = 0.f;
			}
			if (r <= 1.f && g >= 1.f && b == 0.f)
			{
				g = 1.f;
				r -= 0.0005f;
			}
			if (r <= 0.f && g == 1.f && b >= 0.f)
			{
				r = 0.f;
				b += 0.0005f;
			}
			if (r == 0.f && g <= 1.f && b >= 1.f)
			{
				b = 1.f;
				g -= 0.0005f;
			}
			if (r >= 0.f && g <= 0.f && b == 1.f)
			{
				g = 0.f;
				r += 0.0005f;
			}
			if (r >= 1.f && g >= 0.f && b <= 1.f)
			{
				r = 1.f;
				b -= 0.0005f;
			}

			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(r, g, b, 1.f));
			ImGui::BeginChild("##rainbow bar", ImVec2(930.f, 2.f)); 
			ImGui::EndChild(); 
			ImGui::PopStyleColor();

			ImGui::BeginChild("##spacing", ImVec2(0, 20)); {}ImGui::EndChild();

			ImGui::BeginChild("##sidebar", ImVec2(127, 485), false, ImGuiWindowFlags_NoScrollbar);
			{
				style.ButtonTextAlign = ImVec2(0.f, 0.5f);

				ImGui::PushFont(ftabs);
				style.Colors[ImGuiCol_Text] = ImColor(204, 204, 204, 255);
				if (ImGui::Button("LEGITBOT", ImVec2(127.f, 50.0f)))
				{
					page = 1;
				}
				if (ImGui::Button("VISUALS", ImVec2(127.f, 50.0f)))
				{
					page = 2;
				}
				if (ImGui::Button("COLORS", ImVec2(127.f, 50.0f)))
				{
					page = 3;
				}
				if (ImGui::Button("SKINS", ImVec2(127.f, 50.0f)))
				{
					page = 4;
				}
				if (ImGui::Button("MISC", ImVec2(127.f, 50.0f)))
				{
					page = 5;
				}
				ImGui::PopFont();

				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				ImGui::BeginChild("##spacing", ImVec2(0, 40)); {}ImGui::EndChild();
				static char buff[128];
				ImGui::PushItemWidth(115);
				ImGui::InputText("##Input name", buff, 20);
				ImGui::PushItemWidth(127);
				if (ImGui::Button("Save Config", ImVec2(115, 20)))
					Config->Save(buff);
				if (ImGui::Button("Load Config", ImVec2(115, 20)))
					Config->Load(buff);
				if (ImGui::Button("Quit Game", ImVec2(115, 20)))
				{
					Interfaces::g_Engine->ClientCmd_Unrestricted("quit");
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			if (page == 1)
			{
				ImGui::PushItemWidth(150);

				ImGui::BeginChild("#Aimbot Tab", ImVec2(850, 488), false, ImGuiWindowFlags_NoScrollbar);
				{
					ImGui::BeginChild("#aimbotsss", ImVec2(470, 260), true, ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::PushFont(fkek2);

						if (ImGui::Button("W", ImVec2(80, 50)))
						{
							weapontab = 1;
						} ImGui::SameLine();
						if (ImGui::Button("A", ImVec2(80, 50)))
						{
							weapontab = 2;
						} ImGui::SameLine();
						if (ImGui::Button("Z", ImVec2(80, 50)))
						{
							weapontab = 3;
						} ImGui::SameLine();
						if (ImGui::Button("N", ImVec2(80, 50)))
						{
							weapontab = 4;
						} ImGui::SameLine();
						if (ImGui::Button("d", ImVec2(80, 50)))
						{
							weapontab = 5;
						}
						ImGui::PopFont();

						ImGui::Checkbox(("Active"), &g_Options.LegitBot.Enable);
						ImGui::SameLine(95);
						ImGui::Checkbox(("Auto Pistol"), &g_Options.LegitBot.AutoPistol);
						ImGui::SameLine(195);
						ImGui::Hotkey(("Key##LegitBot"), &g_Options.LegitBot.MainKey, ImVec2(110, 20));

						if (weapontab == 1)
						{
		     				ImGui::Text("Rifle");
							ImGui::Separator();
							ImGui::BeginChild("##Rifle settings", ImVec2(230, 120), false);
							{
								ImGui::Spacing();

								ImGui::SliderFloat(("##Smoaoth2121"), &g_Options.LegitBot.MainSmooth, 1.00f, 30.00f, "Smooth: %.1f");

								ImGui::SliderFloat(("##FOV##fdfd"), &g_Options.LegitBot.Mainfov, 0.00f, 30.00f, "FOV: %.1f");

								ImGui::SliderFloat(("##RCS Xvferg"), &g_Options.LegitBot.main_recoil_min, 0.00f, 100.00f, "RCS  X: %.0f");

								ImGui::SliderFloat(("##RCS ffewfY"), &g_Options.LegitBot.main_recoil_max, 0.00f, 100.00f, "RCS  Y: %.0f");

							}ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("##Rifle hitbox", ImVec2(230, 105), false);
							{
								ImGui::Checkbox(("Head"), &g_Options.LegitBot.FilterMainWeapons.Headwpmain);		ImGui::SameLine(90); ImGui::Checkbox(("Neck"), &g_Options.LegitBot.FilterMainWeapons.Neckwpmain);
								ImGui::Checkbox(("Chest"), &g_Options.LegitBot.FilterMainWeapons.Chestwpmain);		ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.FilterMainWeapons.Stomachwpmain);
								ImGui::Checkbox(("Multi Bone"), &g_Options.LegitBot.FilterMainWeapons.multiboneswpmain);

							}
							ImGui::EndChild();
						}



						if (weapontab == 2)
						{

							

							ImGui::Text("Pistol");
							ImGui::Separator();
							ImGui::BeginChild("##Pistol settings", ImVec2(230, 120), false);
							{
								ImGui::Spacing();

								ImGui::SliderFloat(("##Smo132oth21213"), &g_Options.LegitBot.PistolSmooth, 1.00f, 30.00f, "Smooth: %.1f");

								ImGui::SliderFloat(("##FOVfwef"), &g_Options.LegitBot.Pistolfov, 0.00f, 30.00f, "FOV: %.1f");

								ImGui::SliderFloat(("##RfewCS X"), &g_Options.LegitBot.pistol_recoil_min, 0.00f, 100.00f, "RCS  X: %.0f");

								ImGui::SliderFloat(("##RCfdsS Y"), &g_Options.LegitBot.pistol_recoil_max, 0.00f, 100.00f, "RCS  Y: %.0f");

							}ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("##Pistol hitbox", ImVec2(230, 105), false);
							{
								ImGui::Checkbox(("Head"), &g_Options.LegitBot.FilterPistolWeapons.Headwppis);		ImGui::SameLine(90); ImGui::Checkbox(("Neck"), &g_Options.LegitBot.FilterPistolWeapons.Neckwppis);
								ImGui::Checkbox(("Chest"), &g_Options.LegitBot.FilterPistolWeapons.Chestwppis);	ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.FilterPistolWeapons.Stomachwppis);
								ImGui::Checkbox(("Multi Bone"), &g_Options.LegitBot.FilterPistolWeapons.multiboneswppis);

							}
							ImGui::EndChild();
						}

						if (weapontab == 3)
						{


							ImGui::Text("Sniper");
							ImGui::Separator();
							ImGui::BeginChild("##Sniper settings", ImVec2(230, 120), false);
							{
								ImGui::Spacing();

								ImGui::SliderFloat(("##Smofew2zloth"), &g_Options.LegitBot.SniperSmooth, 1.00f, 30.00f, "Smooth: %.1f");

								ImGui::SliderFloat(("##FOddqcsV"), &g_Options.LegitBot.Sniperfov, 0.00f, 30.00f, "FOV: %.1f");

								ImGui::SliderFloat(("##RClfewfewweS X"), &g_Options.LegitBot.sniper_recoil_min, 0.00f, 100.00f, "RCS  X: %.0f");

								ImGui::SliderFloat(("##lRCfwef3eS Y"), &g_Options.LegitBot.sniper_recoil_max, 0.00f, 100.00f, "RCS  Y: %.0f");

							}ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("##Sniper hitbox", ImVec2(230, 105), false);
							{
								ImGui::Checkbox(("Head"), &g_Options.LegitBot.FilterSniperWeapons.HeadwpSnip);		ImGui::SameLine(90); ImGui::Checkbox(("Neck"), &g_Options.LegitBot.FilterSniperWeapons.NeckwpSnip);
								ImGui::Checkbox(("Chest"), &g_Options.LegitBot.FilterSniperWeapons.ChestwpSnip);	ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.FilterSniperWeapons.StomachwpSnip);
								ImGui::Checkbox(("Multi Bone"), &g_Options.LegitBot.FilterSniperWeapons.multiboneswpSnip);

							}
							ImGui::EndChild();
						}

						if (weapontab == 4)
						{

							ImGui::Text("SMG");
							ImGui::Separator();
							ImGui::BeginChild("#SMG settings", ImVec2(230, 120), false);
							{
								ImGui::Spacing();

								ImGui::SliderFloat(("##Smzz55ooth"), &g_Options.LegitBot.smg_Smooth, 1.00f, 30.00f, "Smooth: %.1f");

								ImGui::SliderFloat(("##FOwed5V"), &g_Options.LegitBot.smg_fov, 0.00f, 30.00f, "FOV: %.1f");

								ImGui::SliderFloat(("##45jRC5S X"), &g_Options.LegitBot.smg_recoil_min, 0.00f, 100.00f, "RCS  X: %.0f");

								ImGui::SliderFloat(("##R54C5S Y"), &g_Options.LegitBot.smg_recoil_max, 0.00f, 100.00f, "RCS  Y: %.0f");
								
							}ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("#SMG hitbox", ImVec2(230, 105), false);
							{
								ImGui::Checkbox(("Head"), &g_Options.LegitBot.FiltersmgWeapons.Headwpsmg);		ImGui::SameLine(90); ImGui::Checkbox(("Neck"), &g_Options.LegitBot.FiltersmgWeapons.Neckwpsmg);
								ImGui::Checkbox(("Chest"), &g_Options.LegitBot.FiltersmgWeapons.Chestwpsmg);	ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.FiltersmgWeapons.Stomachwpsmg);
								ImGui::Checkbox(("Multi Bone"), &g_Options.LegitBot.FiltersmgWeapons.multiboneswpsmg);

							}
							ImGui::EndChild();
						}

						if (weapontab == 5)
						{

							ImGui::Text("Heavy");
							ImGui::Separator();
							ImGui::BeginChild("##Heavy settings", ImVec2(230, 120), false);
							{
								ImGui::Spacing();

								ImGui::SliderFloat(("##Sjdidmo1oth"), &g_Options.LegitBot.heavy_wp_Smooth, 1.00f, 30.00f, "Smooth: %.1f");

								ImGui::SliderFloat(("##FOfewfw3erf1V"), &g_Options.LegitBot.heavy_wp_fov, 0.00f, 30.00f, "FOV: %.1f");

								ImGui::SliderFloat(("##Rfsd1CS X"), &g_Options.LegitBot.heavy_wp_recoil_min, 0.00f, 100.00f, "RCS  X: %.0f");

								ImGui::SliderFloat(("##Rrfew1CS Y"), &g_Options.LegitBot.heavy_wp_recoil_max, 0.00f, 100.00f, "RCS  Y: %.0f");

							}ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("##Heavy hitbox", ImVec2(230, 105), false);
							{
								ImGui::Checkbox(("Head"), &g_Options.LegitBot.FilterheavyWeapons.Headwphv);		ImGui::SameLine(90); ImGui::Checkbox(("Neck"), &g_Options.LegitBot.FilterheavyWeapons.Neckwphv);
								ImGui::Checkbox(("Chest"), &g_Options.LegitBot.FilterheavyWeapons.Chestwphv);	ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.FilterheavyWeapons.Stomachwphv);
								ImGui::Checkbox(("Multi Bone"), &g_Options.LegitBot.FilterheavyWeapons.multibonewphv);

							}ImGui::EndChild();
						}
					}
					ImGui::EndChild();

					ImGui::SameLine(0.0, 15.0f);

					ImGui::BeginChild("##triggerbot", ImVec2(260, 260), true, ImGuiWindowFlags_NoScrollbar);
					{
						ImGui::Text(("Triggerbot"));
						ImGui::Separator();
						ImGui::Checkbox(("Active"), &g_Options.LegitBot.Triggerbot.Enabled);
						ImGui::SameLine(110);
						ImGui::Hotkey("Key##triggerke1y11", &g_Options.LegitBot.Triggerbot.Key, ImVec2(110, 20));

						ImGui::Spacing();

						ImGui::SliderFloat(("##Delay"), &g_Options.LegitBot.Triggerbot.Delay, 1.f, 150.f, "Delay: %.0f");
						ImGui::SliderFloat(("##Hitchance"), &g_Options.LegitBot.Triggerbot.hitchance, 1.f, 100.f, "Min Hitchance: %.0f");

						ImGui::Text("Filter");

						ImGui::Checkbox(("Head"), &g_Options.LegitBot.Triggerbot.Filter.Head);		ImGui::SameLine(90); ImGui::Checkbox(("Arms"), &g_Options.LegitBot.Triggerbot.Filter.Arms);
						ImGui::Checkbox(("Chest"), &g_Options.LegitBot.Triggerbot.Filter.Chest);		ImGui::SameLine(90); ImGui::Checkbox(("Stomach"), &g_Options.LegitBot.Triggerbot.Filter.Stomach);
						ImGui::Checkbox(("Legs"), &g_Options.LegitBot.Triggerbot.Filter.Legs);


					}
					ImGui::EndChild();

					ImGui::BeginChild("Backtrack", ImVec2(195, 110), true);
					{
						ImGui::Text("Backtrack");
						ImGui::Separator();
						ImGui::Checkbox(("Active"), &g_Options.Backtrack.backtrackenable);
						ImGui::SameLine(110);
						ImGui::Checkbox(("Dots"), &g_Options.Backtrack.backtrackdots);
						ImGui::SliderInt("##Max Ticks", &g_Options.Backtrack.backtrackticks, 1, 12, "Max Ticks: %.0f");
					}
					ImGui::EndChild();

					ImGui::SameLine(0.0, 15.0f);

					ImGui::BeginChild("Legit AA", ImVec2(195, 110), true);
					{
						ImGui::Text("Legit Anti-Aim");
						ImGui::Separator();
						ImGui::Checkbox(("Active"), &g_Options.LegitAA.legitaaenable);
						ImGui::SameLine(110);
						ImGui::PushItemWidth(100);
						ImGui::Combo(XorStr("##LegitAAType"), &g_Options.LegitAA.legitaatype, LegitAAType, ARRAYSIZE(LegitAAType));
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}

			ImGui::PushItemWidth(100);

			if (page == 2)
			{
				ImGui::PushItemWidth(100);

				ImGui::BeginChild("Visuals", ImVec2(850, 488), false, ImGuiWindowFlags_NoScrollbar);
				{
					ImGui::Checkbox(("Active"), &g_Options.Visuals.Active);

					ImGui::BeginChild("##PLAYER ESP", ImVec2(255, 420), true);
					{
						ImGui::Text("Player ESP");
						ImGui::Separator();

							ImGui::Checkbox(XorStr("Bounding Box"), &g_Options.Visuals.Box);
							ImGui::SameLine(130);
							ImGui::PushItemWidth(100);
							ImGui::Combo(XorStr("##Box Type"), &g_Options.Visuals.Boxtype, "Full\0\rCorners\0\r3D\0\0", 100);
							ImGui::Checkbox(XorStr("Teammate"), &g_Options.Visuals.TeamESP);
							if (g_Options.Visuals.Boxtype == 0)
							{
								ImGui::Checkbox(XorStr("Fill Box"), &g_Options.Visuals.FillESP);
								ImGui::SameLine(130);
								ImGui::PushItemWidth(100);
								ImGui::SliderFloat(XorStr("##alphafill"), &g_Options.Visuals.FillESPAmount, 0.f, 255.f, "Alpha: %.0f");
							}
							else if (g_Options.Visuals.Boxtype == 1)
							{
								ImGui::Checkbox(XorStr("Fill Box"), &g_Options.Visuals.FillESP);
								ImGui::SameLine(130);
								ImGui::PushItemWidth(100);
								ImGui::SliderFloat(XorStr("##alphafill"), &g_Options.Visuals.FillESPAmount, 0.f, 255.f, "Alpha: %.0f");
							}
							else if (g_Options.Visuals.Boxtype == 2)
							{
								g_Options.Visuals.FillESP = false;
								g_Options.Visuals.FillESPAmount = 0.f;
							}
							ImGui::Checkbox(XorStr("Name"), &g_Options.Visuals.Name);
							ImGui::Checkbox(XorStr("Weapon"), &g_Options.Visuals.Weapon);
							ImGui::SameLine(130);
							ImGui::Combo(XorStr("##WeaponType"), &g_Options.Visuals.WeaponType, WeaponType, ARRAYSIZE(WeaponType));
							ImGui::Checkbox(XorStr("Snap Lines"), &g_Options.Visuals.Snapline);
							ImGui::Checkbox(XorStr("Health"), &g_Options.Visuals.Health);
							ImGui::Checkbox(XorStr("Armor Bar"), &g_Options.Visuals.Armor);
							ImGui::SameLine(130);
							ImGui::Combo(XorStr("##Armor 2"), &g_Options.Visuals.Armor2, armor, ARRAYSIZE(armor));
							ImGui::Checkbox(XorStr("Skeleton"), &g_Options.Visuals.Skeleton);
							if (g_Options.Visuals.Skeleton)
							{
								ImGui::SameLine(130);
								ImGui::Checkbox(XorStr("HP Based"), &g_Options.Visuals.Skeletonhealth);
							}

							ImGui::Checkbox(XorStr("Flags"), &g_Options.Visuals.Flags); ImGui::SameLine(130);
							ImGui::Checkbox(XorStr("Callout"), &g_Options.Visuals.Callout);
							ImGui::Checkbox(XorStr("Distance"), &g_Options.Visuals.Distance); ImGui::SameLine(130);
							ImGui::Checkbox(XorStr("Money"), &g_Options.Visuals.Money);

							ImGui::Checkbox(XorStr("Damage Indicator"), &g_Options.Visuals.Damageindicator);
							ImGui::Checkbox(XorStr("Aim Direction"), &g_Options.Visuals.Aimdirection);
							ImGui::SameLine(130);
							ImGui::PushItemWidth(100);
							ImGui::SliderInt(XorStr("##Aim Direction Length"), &g_Options.Visuals.Aimdirectionlength, 0, 300);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##ESP_COLUMN_2", ImVec2(255, 420), false);
					{

						ImGui::BeginChild("World ESP", ImVec2(255, 240), true);
						{
							ImGui::Text("World ESP Box"); ImGui::Separator();
							ImGui::Checkbox(XorStr("Bomb Info"), &g_Options.Visuals.Bomb);
							ImGui::Checkbox(XorStr("Grenades"), &g_Options.Visuals.Grenades);
							ImGui::SameLine(130);
							ImGui::Combo(XorStr("##GrenadeType"), &g_Options.Visuals.GrenadeType, grenades, ARRAYSIZE(grenades));
							ImGui::Checkbox(XorStr("Grenade Trajectory"), &g_Options.Visuals.Grenadetrajectory);
							ImGui::Checkbox(XorStr("Dropped Guns"), &g_Options.Visuals.Droppedguns);
							ImGui::SameLine(130);
							ImGui::PushItemWidth(100);
							ImGui::Combo(XorStr("##Dropped Guns Type"), &g_Options.Visuals.Droppedgunstype, droppedWeapons, ARRAYSIZE(droppedWeapons));
							ImGui::Checkbox(XorStr("Night Mode"), &g_Options.Visuals.Nightmode);
							ImGui::SameLine(130);
							ImGui::Checkbox(XorStr("Ambient Light"), &g_Options.Visuals.Ambientlight);
							ImGui::Checkbox(XorStr("Bullet Tracers"), &g_Options.Visuals.Bullettracers);
							ImGui::PushItemWidth(90);
							ImGui::SliderFloat(XorStr("##Tracer Duration"), &g_Options.Visuals.Bullettracersduration, 0.f, 10.f, "Duration: %.2f");
							ImGui::SameLine(120);
							ImGui::SliderFloat(XorStr("##Tracer Width"), &g_Options.Visuals.Bullettracerswidth, 0.f, 10.f, "Width: %.2f");

						}ImGui::EndChild();

						ImGui::BeginChild("##Crosshairs Box", ImVec2(255, 172), true);
						{
							ImGui::PushItemWidth(100);
							ImGui::Text("Crosshairs"); ImGui::Separator();
							ImGui::Checkbox(XorStr("Sniper"), &g_Options.Visuals.Snipercrosshair);
							ImGui::SameLine(110);
							ImGui::Combo(XorStr("##Sniper Crosshair Type"), &g_Options.Visuals.Snipercrosshairtype, snipercrosshair, ARRAYSIZE(snipercrosshair));
							ImGui::Checkbox(XorStr("Recoil"), &g_Options.Visuals.Recoilcrosshair);
							ImGui::SameLine(110);
							ImGui::Combo(XorStr("##Recoil Crosshair Type"), &g_Options.Visuals.Recoilcrosshairtype, rccrosshair, ARRAYSIZE(rccrosshair));
							ImGui::Checkbox(XorStr("Spread"), &g_Options.Visuals.Spreadcrosshair);
							ImGui::SameLine(110);
							ImGui::SliderFloat(XorStr("##Spread Crosshair Alpha"), &g_Options.Visuals.Spreadcrosshairalpha, 0.f, 255.f, "Alpha: %.0f");
							ImGui::PushItemWidth(85);
							ImGui::Checkbox(XorStr("Hitmarker"), &g_Options.Visuals.Hitmarker);
							ImGui::SameLine(110);
							ImGui::Combo(XorStr("Sound"), &g_Options.Visuals.Hitsound, HitmarkSound, ARRAYSIZE(HitmarkSound));
						}ImGui::EndChild();

					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##ESP_COLUMN_3", ImVec2(255, 420), false);
					{
						ImGui::BeginChild("##Chams Box", ImVec2(255, 215), true);
						{
							ImGui::Text("Chams");
							ImGui::Separator();
							ImGui::Checkbox(XorStr("Player##1"), &g_Options.Visuals.Chamsplayer);
							if (g_Options.Visuals.Chamsplayer || g_Options.Visuals.Chamsteam)
							{
								ImGui::SameLine(110);
								ImGui::Checkbox(("HP Based"), &g_Options.Visuals.Chamshealth);
							}
							ImGui::Checkbox(XorStr("Team##342"), &g_Options.Visuals.Chamsteam);
							ImGui::Checkbox(XorStr("XQZ"), &g_Options.Visuals.Chamsxqz);
							ImGui::Text("Player Style"); ImGui::SameLine(120); ImGui::PushItemWidth(100);
							ImGui::Combo(XorStr("##ChamsStyle"), &g_Options.Visuals.Chamsstyle, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\0", -1);
							ImGui::Text("Hands Style"); ImGui::SameLine(120);
							ImGui::Combo(XorStr("##Hands Style"), &g_Options.Visuals.Handstyle, HandsMode, ARRAYSIZE(HandsMode));
							ImGui::Text("Weapon Style"); ImGui::SameLine(120);
							ImGui::Combo(XorStr("##Weapon Style"), &g_Options.Visuals.Weaponstyle, chamswp, ARRAYSIZE(chamswp));
						}ImGui::EndChild();

						ImGui::BeginChild("##Glow ESP Box", ImVec2(255, 160), true);
						{
							ImGui::Text("Glow ESP");
							ImGui::Separator();
							ImGui::PushItemWidth(100);
							ImGui::Checkbox(XorStr("Player##2"), &g_Options.Visuals.Glowplayer);
							ImGui::SameLine(120);
							ImGui::SliderFloat(XorStr("##Glow Player Alpha"), &g_Options.Visuals.Glowplayeralpha, 0.f, 255.f, "Alpha: %.0f");
							ImGui::Checkbox(XorStr("Team##242"), &g_Options.Visuals.Glowteam);
							ImGui::SameLine(120);
							ImGui::SliderFloat(XorStr("##Glow Team Alpha"), &g_Options.Visuals.Glowteamalpha, 0.f, 255.f, "Alpha: %.0f");
							ImGui::Checkbox(XorStr("Weapons"), &g_Options.Visuals.Glowweapon);
							ImGui::SameLine(120);
							ImGui::SliderFloat(XorStr("##Glow Weapon Alpha"), &g_Options.Visuals.Glowweaponalpha, 0.f, 255.f, "Alpha: %.0f");
							ImGui::Checkbox(XorStr("Bomb##1rfew34"), &g_Options.Visuals.Glowbomb);
							ImGui::SameLine(120);
							ImGui::SliderFloat(XorStr("##Glow Bomb Alpha"), &g_Options.Visuals.Glowbombalpha, 0.f, 255.f, "Alpha: %.0f");
						}ImGui::EndChild();

					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}

			if (page == 3)
			{

				static int selectedItem = 0;
				ImVec2 lastCursor = ImGui::GetCursorPos();

				ImGui::ListBoxHeader("##0", ImVec2(240, 460));
				for (int i = 0; i < Global::ColorsForPicker1.size(); i++)
				{
					bool selected = i == selectedItem;

					if (ImGui::Selectable(Global::ColorsForPicker1[i].Name, selected))
						selectedItem = i;
				}
				ImGui::ListBoxFooter();

				ImGui::SameLine(0.0f, 15.0f);
				ImGui::BeginChild("##spacing", ImVec2(120, 0)); {}ImGui::EndChild();

				ImGui::BeginChild("#generalcolors", ImVec2(450, 460), false, ImGuiWindowFlags_NoScrollbar);
				{


					ImGui::BeginChild("#colorpreview", ImVec2(420, 300), false);
					{

						ColorP col = Global::ColorsForPicker1[selectedItem];
						ImGui::ColorPicker("##COLOR_PICKER", col.Ccolor);

					}ImGui::EndChild();


					ImGui::BeginChild("#ColorsChild", ImVec2(420, 105), false);
					{
						ImGui::Spacing();
						ImGui::Spacing();
						ColorP col = Global::ColorsForPicker1[selectedItem];
						int r = (col.Ccolor[0] * 255.f);
						int g = (col.Ccolor[1] * 255.f);
						int b = (col.Ccolor[2] * 255.f);
						ImGui::PushItemWidth(290);
						ImGui::SliderInt("##red", &r, 0, 255, "Red Amount: %.0f"); ImGui::SameLine();
						ImGui::Spacing();
						ImGui::SliderInt("##green", &g, 0, 255, "Green Amount: %.0f"); ImGui::SameLine();
						ImGui::Spacing();
						ImGui::SliderInt("##blue", &b, 0, 255, "Blue Amount: %.0f"); ImGui::SameLine();
						ImGui::PopItemWidth();
						col.Ccolor[0] = r / 255.0f;
						col.Ccolor[1] = g / 255.0f;
						col.Ccolor[2] = b / 255.0f;

					}ImGui::EndChild();

				}ImGui::EndChild();


			}

			if (page == 4) // (*g_ClientState)->ForceFullUpdate();
			{
				ImGui::BeginChild("Skinchanger", ImVec2(850, 488), false, ImGuiWindowFlags_NoScrollbar);
				{

					static char filterWeapons[32];
					static char filterSkins[32];

					ImGui::Checkbox(("Active"), &g_Options.Skinchanger.Enabled);
					//ImGui::SameLine();
					//ImGui::Checkbox(("Skin ID List"), &g_Options.Skinchanger.SkinFilter);


					ImGui::BeginChild("first child", ImVec2(195, 420), true);
					{
						ImGui::Text("Weapon Selector");
						ImGui::Separator();

						ImGui::PushItemWidth(-1);
						ImGui::InputText(("##FILTERWEAPONS"), filterWeapons, IM_ARRAYSIZE(filterWeapons));
						ImGui::PopItemWidth();
						ImGui::ListBoxHeader(("##GUNS"), ImVec2(-1, -1));
						for (auto it : ItemDefinitionIndexMap)
						{
							bool isDefault = (int)it.first < 0;
							if (!isDefault && !Contains(ToLower(std::string(filterWeapons)), ToLower(ItemDefinitionIndexMap.at(it.first).entityName)))
								continue;

							if (IsUtility(it.first))
								continue;

							const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);

							std::string formattedName;

							formattedName = ItemDefinitionIndexMap.at(it.first).entityName;
							if (ImGui::Selectable(formattedName.c_str(), item_selected))
							{
								g_Options.Menu.currentWeapon = (int)it.first;
							}
						}
						ImGui::ListBoxFooter();
					}ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("second child", ImVec2(195, 420), true);
					{
						ImGui::Text("Paintkit Selector");
						ImGui::Separator();

						std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
						{
							ImGui::PushItemWidth(-1);
							ImGui::InputText(("##FILTERSKINS"), filterSkins, IM_ARRAYSIZE(filterSkins));
							ImGui::InputInt(("##customID"), &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);
							ImGui::PopItemWidth();
							ImGui::ListBoxHeader(("##SKINS"), ImVec2(-1, -1));
							std::string skinStr = "";
							int curItem = -1;
							int s = 0;
							for (auto skin : g_Options.Skinchanger.weaponSkins[skinName])
							{
								int pk = g_Options.Skinchanger.skinMap[skin].paintkit;
								if (pk == g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin)
									curItem = s;
								bool selected = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin == pk;


								skinStr = g_Options.Skinchanger.skinNames[g_Options.Skinchanger.skinMap[skin].tagName].c_str();
								skinStr += " | ";
								skinStr += std::to_string(g_Options.Skinchanger.skinMap[skin].paintkit);
								if (!Contains(ToLower(std::string(filterSkins)), ToLower(skinStr)))
									continue;
								if (ImGui::Selectable(skinStr.c_str(), selected))
								{
									pk = g_Options.Skinchanger.skinMap[skin].paintkit;
									g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin = pk;
									FullUpdate();
									break;
								}
								s++;
							}

							ImGui::ListBoxFooter();
						}
					}ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("third child", ImVec2(195, 420), true);
					{
						ImGui::ListBoxHeader("##KNIVES", ImVec2(-1, -1));
						{
							ImGui::Text("Knife Changer");
							ImGui::Separator();

							if (ImGui::RadioButton(("Default"), &g_Options.Skinchanger.knifemodel, 0))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Bayonet"), &g_Options.Skinchanger.knifemodel, 1))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Flip Knife"), &g_Options.Skinchanger.knifemodel, 2))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Gut Knife"), &g_Options.Skinchanger.knifemodel, 3))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Karambit"), &g_Options.Skinchanger.knifemodel, 4))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("M9 Bayonet"), &g_Options.Skinchanger.knifemodel, 5))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Huntsman"), &g_Options.Skinchanger.knifemodel, 6))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Falchion"), &g_Options.Skinchanger.knifemodel, 7))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Bowie"), &g_Options.Skinchanger.knifemodel, 8))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Butterfly Knife"), &g_Options.Skinchanger.knifemodel, 9))
							{
								FullUpdate();
							}
							if (ImGui::RadioButton(("Shadow Daggers"), &g_Options.Skinchanger.knifemodel, 10))
							{
								FullUpdate();
							}

						}
						ImGui::ListBoxFooter();
					}ImGui::EndChild();

					/*
					if (ImGui::Button(("Apply Changes"), ImVec2(100, 40)))
					{
						FullUpdate();
					}
					*/

				}ImGui::EndChild();
			}

			ImGui::PushItemWidth(100);

			if (page == 5)
			{
				ImGui::PushItemWidth(100);
				ImGui::BeginChild("##Misc Column 1", ImVec2(255, 420), false);
				{
					ImGui::BeginChild("##General Box", ImVec2(255, 260), true);
					{
						ImGui::Text("General");
						ImGui::Separator();

						if (ImGui::Checkbox(("Anti AFK"), &g_Options.Misc.afkbot))
						{
							if (g_Options.Misc.afkbot)
							{

								Interfaces::g_Engine->ClientCmd_Unrestricted("+forward;+moveleft;+left");

							}
							else if (!g_Options.Misc.afkbot)
							{
								Interfaces::g_Engine->ClientCmd_Unrestricted("-forward;-moveleft;-left");
							}
						}
						//ImGui::Checkbox(("Auto Accept (test)"), &g_Options.Misc.AutoAccept);
						ImGui::PushItemWidth(100);
						ImGui::Combo(("Auto Strafe"), &g_Options.Misc.AutoStrafe, autostrafe, ARRAYSIZE(autostrafe));
						ImGui::Checkbox(("Bunny Hop"), &g_Options.Misc.Bhop);
						ImGui::Checkbox(("Fake Lag"), &g_Options.Misc.FakeLag);
						ImGui::SameLine(110);
						ImGui::SliderInt(XorStr("##Fake Lag"), &g_Options.Misc.FakeLagSlider, 0.f, 14.f, "Amount: %.0f");
						//ImGui::Checkbox(("Legit AA"), &g_Options.Backtrack.legitaa);
						ImGui::Checkbox(("Purchase Event Logs"), &g_Options.Misc.eventlogs2);
						ImGui::Checkbox(("Rank Revealer"), &g_Options.Misc.ServerRankRevealAll);
						ImGui::Checkbox(("Spectator List"), &g_Options.Misc.SpecList);
					}ImGui::EndChild();

					ImGui::BeginChild("##FOV Box", ImVec2(255, 105), true);
					{
						ImGui::Text("FOV Changer");
						ImGui::Separator();
						ImGui::PushItemWidth(100);
						ImGui::Checkbox(XorStr("FOV"), &g_Options.Visuals.FOV);
						ImGui::SameLine(130);
						ImGui::SliderFloat(XorStr("##FOV"), &g_Options.Visuals.FOVChanger, 0, 60, "Amount: %.0f");
						ImGui::Checkbox(XorStr("VFOV"), &g_Options.Visuals.VFOV);
						ImGui::SameLine(130);
						ImGui::SliderFloat(XorStr("##VFOV"), &g_Options.Visuals.VFOVChanger, 68, 120, "Amount: %.0f");
					}ImGui::EndChild();


				}ImGui::EndChild();

				ImGui::SameLine(0.0f, 15.0f);

				ImGui::BeginChild("##Misc Column 2", ImVec2(255, 420), false);
				{
					ImGui::PushItemWidth(100);

					ImGui::BeginChild("##Thirdperson Box", ImVec2(255, 108), true);
					{
						ImGui::Text("Thirdperson");
						ImGui::Separator();
						ImGui::PushItemWidth(100);
						ImGui::Checkbox(XorStr("Active"), &g_Options.Visuals.Thirdperson);
						ImGui::SameLine(90);
						ImGui::Hotkey(XorStr("Key##TPfdf"), &g_Options.Visuals.Thirdpersonkey, ImVec2(110, 20));
						ImGui::Checkbox(XorStr("Disable On Grenade"), &g_Options.Visuals.Thirdpersongrenade);
					}ImGui::EndChild();

					ImGui::BeginChild("##Radar Box", ImVec2(255, 132), true);
					{
						ImGui::Text("Radar");
						ImGui::Separator();
						ImGui::Checkbox("Radar Window", &g_Options.Misc.radarwindow);
						ImGui::PushItemWidth(100);
						ImGui::SliderFloat("##radrzom", &g_Options.Misc.radrzoom, 0.f, 4.f, "Zoom: %.2f"); ImGui::PushItemWidth(80);
						ImGui::SliderFloat("##radralpha", &g_Options.Misc.radralpha, 0.f, 255.f, "Alpha: %.0f"); ImGui::SameLine(110);
						ImGui::SliderInt(("##Size"), &g_Options.Misc.radrsize, 200, 500, "Size: %.0f");
					}ImGui::EndChild();

					ImGui::BeginChild("##Removals Box", ImVec2(255, 160), true);
					{
						ImGui::Text("Removals");
						ImGui::Separator();
						ImGui::PushItemWidth(100);
						ImGui::Combo(XorStr("Modify Smoke"), &g_Options.Visuals.Smoke, smoke, ARRAYSIZE(smoke));
						ImGui::Checkbox(XorStr("No Flash"), &g_Options.Visuals.Noflash); ImGui::SameLine(130);
						ImGui::Checkbox(XorStr("No Scope"), &g_Options.Visuals.Noscopeborder);
						ImGui::Checkbox(XorStr("No Visual Recoil"), &g_Options.Visuals.Novisualrecoil);
						ImGui::Checkbox(XorStr("No Post Processing"), &g_Options.Visuals.Nopostprocess);

					}ImGui::EndChild();

				}ImGui::EndChild();

				ImGui::SameLine(0.0f, 15.0f);

				ImGui::BeginChild("##Misc Column 3", ImVec2(255, 420), false);
				{

					ImGui::BeginChild("##Name Box", ImVec2(255, 160), true);
					{
						ImGui::Text("Name Changer");
						ImGui::Separator();
						ImGui::PushItemWidth(190);
						static char nickname[127] = "";
						ImGui::InputText("##NICKNAME", nickname, 127);
						ImGui::PopItemWidth();
						if (ImGui::Button("Set Name"))
						{
							NameChanger::SetName(std::string(nickname).c_str());
						}
						if (ImGui::Button("Blank Name"))
						{
							NameChanger::SetName("\n\xAD\xAD\xAD");
						}
						if (ImGui::Button("shit paste Name"))
						{
							static char nameShitPaste[11] = "shit paste";
							NameChanger::SetName(std::string(nameShitPaste).c_str());
						}

					}ImGui::EndChild();

					ImGui::BeginChild("##Clantag Box", ImVec2(255, 140), true);
					{
						ImGui::Text("Clantag Changer");
						ImGui::Separator();
						ImGui::PushItemWidth(190);
						static char ClanChanger[127] = "";
						ImGui::InputText(("##ClantagINPUT"), ClanChanger, 127, ImGuiInputTextFlags_AllowTabInput);
						if (ImGui::Button(("Set Clantag")))
						{
							SetClantag(ClanChanger);
						}
						ImGui::Checkbox("shitpaste Clantag", &g_Options.Misc.syncclantag);
					}ImGui::EndChild();

				}ImGui::EndChild();
			}

			if (g_Options.Skinchanger.SkinFilter)
			{
				SkinFilter();
			}

		}ImGui::End();

	}

}