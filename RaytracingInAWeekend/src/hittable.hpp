#pragma once
#include "ray.hpp"
#include <glm/glm.hpp>

class AABB;
class Material;
struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	float u, v;
	Material* mat_ptr = nullptr;
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
	virtual bool BoundingBox(const float time0, const float time1, AABB& box) const = 0;
};
