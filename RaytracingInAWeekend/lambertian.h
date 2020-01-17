#pragma once
#include "material.h"

#include "vec3.h"
#include "random.h"

class lambertian :public material
{
public:
	lambertian(const vec3& albedo) : albedo(albedo) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	
	vec3 albedo;
};