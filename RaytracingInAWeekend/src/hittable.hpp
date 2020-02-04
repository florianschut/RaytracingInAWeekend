#pragma once
#include "ray.hpp"
#include <glm/glm.hpp>

class Material;
struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material* mat_ptr;
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
};
