#pragma once
#include "hittable.hpp"

class Sphere: public Hittable
{
public:
	Sphere(glm::vec3 center_, float radius_, Material* material): center_(center_), radius_(radius_), mat_ptr(material){};
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
	glm::vec3 center_;
	float radius_;
	Material* mat_ptr;
};
