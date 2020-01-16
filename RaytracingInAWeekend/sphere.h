#pragma once
#include "hittable.h"

class sphere: public hittable
{
public:
	sphere() {}
	sphere(vec3 center, float radius, material* material): center(center), radius(radius), mat_ptr(material){};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const;
	vec3 center;
	float radius;
	material* mat_ptr;
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
			record.mat_ptr = mat_ptr;
			return true;
		}

		t_temp = (-b + sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.point_at_parameter(t_temp);
			record.normal = (record.p - center) / radius;
			record.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

