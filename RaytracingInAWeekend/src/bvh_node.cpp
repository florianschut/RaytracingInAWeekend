#include "bvh_node.hpp"
#include "random.hpp"
#include <iostream>

int BoxXCompare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hittable* a_hittable = *(Hittable**)a;
	Hittable* b_hittable = *(Hittable**)b;

	if (!a_hittable->BoundingBox(0, 0, box_left) || !b_hittable->BoundingBox(0, 0, box_right))
		std::cerr << "No bounding box in BvhNode constructor \n";

	if (box_left.Min().x - box_right.Min().x < 0.0f)
		return -1;

	return 1;
}

int BoxYCompare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hittable* a_hittable = *(Hittable**)a;
	Hittable* b_hittable = *(Hittable**)b;

	if (!a_hittable->BoundingBox(0, 0, box_left) || !b_hittable->BoundingBox(0, 0, box_right))
		std::cerr << "No bounding box in BvhNode constructor \n";

	if (box_left.Min().y - box_right.Min().y < 0.0f)
		return -1;

	return 1;
}

int BoxZCompare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hittable* a_hittable = *(Hittable**)a;
	Hittable* b_hittable = *(Hittable**)b;

	if (!a_hittable->BoundingBox(0, 0, box_left) || !b_hittable->BoundingBox(0, 0, box_right))
		std::cerr << "No bounding box in BvhNode constructor \n";

	if (box_left.Min().z - box_right.Min().z < 0.0f)
		return -1;

	return 1;
}

BvhNode::BvhNode(Hittable** list, int n, float time0, float time1)
{
	auto axis = static_cast<int>(3.f * utility::RandomFloat());
	if (axis == 0)
		qsort(list, n, sizeof(Hittable*), BoxXCompare);
	else if (axis == 1)
		qsort(list, n, sizeof(Hittable*), BoxYCompare);
	else
		qsort(list, n, sizeof(Hittable*), BoxZCompare);

	if (n == 1)
		left_ = right_ = list[0];
	else if (n == 2)
	{
		left_ = list[0];
		right_ = list[1];
	}
	else
	{
		left_ = new BvhNode(list, n / 2, time0, time1);
		right_ = new BvhNode(list + n / 2, n - n / 2, time0, time1);
	}

	AABB box_left, box_right;
	if(!left_->BoundingBox(time0, time1, box_left) || !right_->BoundingBox(time0, time1, box_right))
		std::cerr << "No bounding box in BvhNode constructor \n";

	box_ = AABB::SurroundingBox(box_left, box_right);

}

bool BvhNode::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	if (box_.Hit(r, t_min, t_max))
	{
		HitRecord left_record, right_record;
		const auto hit_left = left_->Hit(r, t_min, t_max, left_record);
		const auto hit_right = right_->Hit(r, t_min, t_max, right_record);
		if (hit_left && hit_right)
		{
			if (left_record.t < right_record.t)
				record = left_record;
			else
				record = right_record;
			
			return true;
		}
		if (hit_left)
		{

			record = left_record;
			return true;
		}
		if (hit_right)
		{
			record = right_record;
			return true;
		}
		return false;
	}
	return false;
}
