#pragma once
#include "ray.hpp"
#include <glm/glm.hpp>

class material;
struct hit_record
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	material* mat_ptr;
};

class hittable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& record) const = 0;
};
