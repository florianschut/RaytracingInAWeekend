#pragma once
#include "hittable.hpp"
class HittableList: public Hittable
{
public:
	HittableList() = default;
	HittableList(Hittable** l, int n) : list_(l), list_size_(n) { };
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;
	Hittable** list_ = nullptr;
	unsigned int list_size_ = 0;
};
