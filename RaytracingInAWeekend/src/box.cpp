#include "box.hpp"
#include "rectangle.hpp"
#include "hittable_list.hpp"
#include "ImGui/imgui.h"

Box::Box(const glm::vec3& p0, const glm::vec3& p1, std::shared_ptr<Material> material): pmin_(p0), pmax_(p1)
{
	rect_list_.Add(std::make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, material));
	rect_list_.Add(std::make_shared<FlipNormals>(std::make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.z, material)));
	rect_list_.Add(std::make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, material));
	rect_list_.Add(std::make_shared<FlipNormals>(std::make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p0.y, material)));
	rect_list_.Add(std::make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, material));
	rect_list_.Add(std::make_shared<FlipNormals>(std::make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p0.x, material)));
}

inline void Box::ImGuiMenu()
{
	if (name_.empty())
		name_ = "Box";
	if (ImGui::CollapsingHeader(name_.c_str()))
	{
		ImGui::Text("Box Settings");
		ImGui::Separator();
	}
}
