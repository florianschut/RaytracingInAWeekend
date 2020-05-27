#include "bvh_node.hpp"
#include "random.hpp"
#include <iostream>
#include <string>


#include "ImGui/imgui.h"


inline bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
	AABB box_a, box_b;
	if (!a->BoundingBox(0, 0, box_a) || !b->BoundingBox(0, 0, box_b))
		std::cerr << "No bounding box in BvhNode constructor \n";
	return box_a.Min()[axis] < box_b.Min()[axis];
	
}
bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 0);
}

bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 1);
}

bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 2);
}

BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, float time0, float time1)
{
	auto axis = static_cast<int>(3.f * utility::RandomFloat());
	auto comparator = (axis == 0) ? BoxXCompare
		: (axis == 1) ? BoxYCompare
		: BoxZCompare;
	
	const size_t object_span = end - start;
	if (object_span == 1)
		left_ = right_ = objects[start];
	else if (object_span == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left_ = objects[start];
			right_ = objects[start + 1];
		}
		else
		{
			left_ = objects[start + 1];
			right_ = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		auto mid = start + object_span / 2;
		left_ = std::make_shared<BvhNode>(objects, start, mid, time0, time1);
		right_ = std::make_shared<BvhNode>(objects, mid, end, time0, time1);
	}

	AABB box_left, box_right;
	if(!left_->BoundingBox(time0, time1, box_left) || !right_->BoundingBox(time0, time1, box_right))
		std::cerr << "No bounding box in BvhNode constructor \n";

	box_ = AABB::SurroundingBox(box_left, box_right);
}

bool BvhNode::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	if (!box_.Hit(r, t_min, t_max))
		return false;

	const auto hit_left = left_->Hit(r, t_min, t_max, rec);
	const auto hit_right = right_->Hit(r, t_min, hit_left ? rec.t : t_max, rec);
	
	return hit_left || hit_right;
}

inline void BvhNode::ImGuiMenu()
{
	if (ImGui::CollapsingHeader("BvH Tree"))
		ImGuiChildren();
}

void BvhNode::ImGuiChildren()
{
	auto left_ptr = std::dynamic_pointer_cast<BvhNode>(left_);
	if (left_ptr)
		left_ptr->ImGuiChildren();
	else
		left_->ImGuiMenu();

	auto right_ptr = std::dynamic_pointer_cast<BvhNode>(right_);
	if (right_ptr)
		right_ptr->ImGuiChildren();
	else
		right_->ImGuiMenu();
}
