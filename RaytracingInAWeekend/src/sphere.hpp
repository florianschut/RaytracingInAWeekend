#pragma once
#include "hittable.hpp"

class Sphere: public Hittable
{
public:
	Sphere(const glm::vec3 center, const float radius, Material* material): center_(center), radius_(radius), material_(material){};
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

private:
	glm::vec3 center_;
	float radius_;
	Material* material_;
};
