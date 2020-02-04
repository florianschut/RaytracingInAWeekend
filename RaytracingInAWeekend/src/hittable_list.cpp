#include "hittable_list.hpp"

bool HittableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	HitRecord temp_record;
	bool hit_anything = false;
	float closest_so_far = t_max;
	for (unsigned int i = 0; i < list_size_; i++)
	{
		if (list[i]->Hit(r, t_min, closest_so_far, temp_record))
		{
			hit_anything = true;
			closest_so_far = temp_record.t;
			record = temp_record;
		}
	}
	return hit_anything;
}