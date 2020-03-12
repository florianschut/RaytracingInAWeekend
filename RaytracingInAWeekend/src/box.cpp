#include "box.hpp"
#include "rectangle.hpp"
#include "hittable_list.hpp"

Box::Box(const glm::vec3& p0, const glm::vec3& p1, Material* material): pmin_(p0), pmax_(p1)
{
	Hittable** list = new Hittable * [6];
	list[0] = new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, material);
	list[1] = new FlipNormals(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, material));
	list[2] = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, material);
	list[3] = new FlipNormals(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, material));
	list[4] = new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, material);
	list[5] = new FlipNormals(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, material));
	rect_list_ = new HittableList(list, 6);
}

