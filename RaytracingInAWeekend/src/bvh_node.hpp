#pragma once
#include <vector>
#include <memory>

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "aabb.hpp"

class HittableList;

class BvhNode : public Hittable
{
public:
	BvhNode(HittableList& list, float time0, float time1): BvhNode(list.objects_, 0, list.objects_.size(), time0, time1){}
	BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, float time0, float time1);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	inline bool BoundingBox(const float time0, const float time1, AABB& box) const override;
	void ImGuiMenu() override;
	
private:
	void ImGuiChildren();
	
	std::shared_ptr<Hittable> left_;
	std::shared_ptr<Hittable> right_;
	AABB box_;
};

bool BvhNode::BoundingBox(const float, const float, AABB& box) const
{
	box = box_;
	return true;
}
