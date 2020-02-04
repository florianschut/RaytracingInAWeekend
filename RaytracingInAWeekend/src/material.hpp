#pragma once
#include "hittable.hpp"

class Ray;

class Material
{
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};
