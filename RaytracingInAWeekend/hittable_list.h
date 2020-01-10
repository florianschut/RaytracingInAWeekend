#pragma once
#include "hittable.h"

class hittable_list: public hittable
{
public:
	hittable_list() {}
	hittable_list(hittable** l, int n) : list(l), list_size(n) { };
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const;
	
	hittable** list = nullptr;
	unsigned int list_size = 0;
};

bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& record) const
{
	hit_record temp_record;
	bool hit_anything = false;
	float closest_so_far = t_max;
	for (int i = 0; i< list_size; i++)
	{
		if(list[i]->hit(r, t_min, closest_so_far, temp_record))
		{
			hit_anything = true;
			closest_so_far = temp_record.t;
			record = temp_record;
		}
	}
	return hit_anything;
}
