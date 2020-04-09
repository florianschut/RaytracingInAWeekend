#include "box.hpp"
#include "rectangle.hpp"
#include "hittable_list.hpp"

Box::Box(const glm::vec3& p0, const glm::vec3& p1, Material* material): pmin_(p0), pmax_(p1)
{
	rect_list_.Add(std::make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, material));
	rect_list_.Add(std::make_shared<FlipNormals>(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, material)));
	rect_list_.Add(std::make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, material));
	rect_list_.Add(std::make_shared<FlipNormals>(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, material)));
	rect_list_.Add(std::make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, material));
	rect_list_.Add(std::make_shared<FlipNormals>(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, material)));
}

