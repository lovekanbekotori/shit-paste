#include "Render.h"

void SkinFilter()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_TitleBg] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(21, 21, 21, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(215, 21, 21, 255);

	style.Alpha = 1.f;
}