#pragma once
#include <vector>
#include <memory>
#include "hittable.hpp"

class HittableList: public Hittable
{
public:
	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }
	
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

	void Clear() { objects_.clear(); }
	void Add(std::shared_ptr<Hittable> object) { objects_.push_back(object); }

	std::vector<std::shared_ptr<Hittable>> objects_ ;
};
