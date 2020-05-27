#include "triangle.hpp"

#include "ImGui/imgui.h"

void Triangle::ImGuiMenu()
{
	if (name_.empty())
		name_ = "Triangle";
	if (ImGui::CollapsingHeader(name_.c_str()))
	{
		ImGui::Text("A Solitary Triangle");
		ImGui::Separator();
	}
}
