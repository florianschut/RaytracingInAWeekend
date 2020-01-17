#pragma once
#include "hittable.h"

class vec3;
class ray;

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
