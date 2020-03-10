#pragma once
#include "hittable.hpp"

class Ray;

class Material
{
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
	virtual glm::vec3 Emitted(float, float, const glm::vec3&) const { return glm::vec3(0); }
};
