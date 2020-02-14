#include "hittable_list.hpp"

#include "aabb.hpp"

bool HittableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	HitRecord temp_record;
	bool hit_anything = false;
	float closest_so_far = t_max;
	for (unsigned int i = 0; i < list_size_; i++)
	{
		if (list_[i]->Hit(r, t_min, closest_so_far, temp_record))
		{
			hit_anything = true;
			closest_so_far = temp_record.t;
			record = temp_record;
		}
	}
	return hit_anything;
}

bool HittableList::BoundingBox(const float time0, const float time1, AABB& box) const
{
	if (list_size_ < 1) return false;
	AABB temp_box;
	auto first_true = list_[0]->BoundingBox(time0, time1, temp_box);
	if (!first_true) return false;
	box = temp_box;
	for (auto i = 1u; i < list_size_; i++)
	{
		if (list_[i]->BoundingBox(time0, time1, temp_box))
			box = AABB::SurroundingBox(box, temp_box);
		else return false;
	}
	return true;
}
