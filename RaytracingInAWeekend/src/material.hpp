#pragma once
#include "hittable.hpp"

class ray;

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};
