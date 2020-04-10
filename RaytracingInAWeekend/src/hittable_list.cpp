#include "hittable_list.hpp"

#include "aabb.hpp"

bool HittableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	HitRecord temp_record;
	auto hit_anything = false;
	auto closest_so_far = t_max;
	
	for (const auto& object: objects_)
	{
		if (object->Hit(r, t_min, closest_so_far, temp_record))
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
	if (objects_.empty()) 
		return false;
	AABB temp_box;
	const auto first_true = objects_[0]->BoundingBox(time0, time1, temp_box);
	if (!first_true) 
		return false;
	
	box = temp_box;
	
	for (const auto& object : objects_)
	{
		if (!object->BoundingBox(time0, time1, temp_box))
			return false;
		box = AABB::SurroundingBox(box, temp_box);
	}
	return true;
}
