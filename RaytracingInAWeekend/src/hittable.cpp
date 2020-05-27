#include "hittable.hpp"

#include <string>

#include "ImGui/imgui.h"

void Hittable::ImGuiMenu()
{
	if (name_.empty())
	{
		name_=typeid(*this).name();
	}
	if (ImGui::CollapsingHeader(name_.c_str()))
	{
		ImGui::Text("This %s doesn't have any settings.", typeid(*this).name());
	}
}
