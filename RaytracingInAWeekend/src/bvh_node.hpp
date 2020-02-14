#pragma once
#include "hittable.hpp"
#include "aabb.hpp"

class BvhNode : public Hittable
{
public:
	BvhNode(Hittable** list, int n, float time0, float time1);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	inline bool BoundingBox(const float time0, const float time1, AABB& box) const override;
	
private:
	Hittable* left_;
	Hittable* right_;
	AABB box_;
};

bool BvhNode::BoundingBox(const float, const float, AABB& box) const
{
	box = box_;
	return true;
}
