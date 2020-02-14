#include "aabb.hpp"

AABB AABB::SurroundingBox(AABB box0, AABB box1)
{
	glm::vec3 min(box0.Min().x < box1.Min().x ? box0.Min().x : box1.Min().x,
		box0.Min().y < box1.Min().y ? box0.Min().y : box1.Min().y,
		box0.Min().z < box1.Min().z ? box0.Min().z : box1.Min().z);
	glm::vec3 max(box0.Max().x > box1.Max().x ? box0.Max().x : box1.Max().x,
		box0.Max().y > box1.Max().y ? box0.Max().y : box1.Max().y,
		box0.Max().z > box1.Max().z ? box0.Max().z : box1.Max().z);
	return AABB(min, max);
}
