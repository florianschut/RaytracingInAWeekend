#pragma once
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "aabb.hpp"

class Box: public Hittable
{
public:
	Box(const glm::vec3& p0, const glm::vec3& p1, Material* material);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float, const float, AABB& box) const override
	{
		box = AABB(pmin_, pmax_);
		return true;
	}

private:
	glm::vec3 pmin_, pmax_;
	HittableList rect_list_;
	
};

inline bool Box::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	return rect_list_.Hit(r, t_min, t_max, record);
}
