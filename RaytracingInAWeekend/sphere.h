#pragma once
#include "hittable.h"

class sphere: public hittable
{
public:
	sphere() {}
	sphere(vec3 center, float radius): center(center), radius(radius){};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const;
	vec3 center;
	float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& record) const
{
	const vec3 oc = r.origin - center;
	const float a = dot(r.direction, r.direction);
	const float b = dot(oc, r.direction);
	const float c = dot(oc, oc) - radius * radius;

	const float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float t_temp = (-b - sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.point_at_parameter(t_temp);
			record.normal = (record.p - center) / radius;
			return true;
		}

		t_temp = (-b + sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.point_at_parameter(t_temp);
			record.normal = (record.p - center) / radius;
			return true;
		}
	}
	return false;
}

