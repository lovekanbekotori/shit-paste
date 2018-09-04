#include "Render.h"


void speclist()
{
			auto& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
			style.Colors[ImGuiCol_TitleBgActive] = ImColor(21, 21, 21, 255);
			style.Colors[ImGuiCol_CloseButton] = ImColor(0, 0, 0, 0);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImColor(0, 0, 0, 0);
			style.Colors[ImGuiCol_CloseButtonActive] = ImColor(0, 0, 0, 0);

			if (ImGui::Begin("Spectator List", &g_Options.Misc.SpecList, ImVec2(200.f, 150.f), 0.0f, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize));
			{
				C_BaseEntity *local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
				RECT scrn = g_Render->GetViewport();
				if (local)
				{
					for (int i = 0; i < Interfaces::g_EntityList->GetHighestEntityIndex(); i++)
					{
						// Get the entity
						C_BaseEntity *pEntity = Interfaces::g_EntityList->GetClientEntity(i);
						player_info_t pinfo;
						if (pEntity && pEntity != local)
						{
							if (Interfaces::g_Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
							{
								HANDLE obs = pEntity->GetObserverTargetHandle();
								if (obs)
								{
									C_BaseEntity *pTarget = Interfaces::g_EntityList->GetClientEntityFromHandle(obs);
									player_info_t pinfo2;

									if (pTarget && pTarget->GetIndex() == local->GetIndex())
									{
										if (Interfaces::g_Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
										{
											pinfo.name[16] = 0;
											strcat(pinfo.name, "");
											puts(pinfo.name);

											char buf[255]; sprintf(buf, "%s <= Spec => %s", pinfo.name);
											ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), pinfo.name);
										}
									}
								}
							}
						}
					}
				}
			}			
			ImGui::End();
}

