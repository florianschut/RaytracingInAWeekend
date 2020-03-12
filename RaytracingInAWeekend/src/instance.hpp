#pragma once
#include "hittable.hpp"
#include "aabb.hpp"

class Translate: public Hittable
{
public:
	Translate(Hittable* hittable, const glm::vec3& offset): hittable_(hittable), offset_(offset){}
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;
private:
	Hittable* hittable_;
	glm::vec3 offset_;
};

inline bool Translate::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	Ray moved_ray(r.Origin() - offset_, r.Direction(), r.Time());
	if (hittable_->Hit(moved_ray, t_min, t_max, record))
	{
		record.p += offset_;
		return true;
	}
	return false;
}

class RotateY: public Hittable
{
public:
	RotateY(Hittable* hittable, float angle);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

private:
	Hittable* hittable_;
	float sin_theta_, cos_theta_;
	bool has_box_;
	AABB box_;
};

inline bool RotateY::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	glm::vec3 origin = r.Origin();
	glm::vec3 direction = r.Direction();
	origin.x = cos_theta_ * r.Origin().x - sin_theta_ * r.Origin().z;
	origin.z = sin_theta_ * r.Origin().x + cos_theta_ * r.Origin().z;
	direction.x = cos_theta_ * r.Direction().x - sin_theta_ * r.Direction().z;
	direction.z = sin_theta_ * r.Direction().x + cos_theta_ * r.Direction().z;
	Ray rotated_ray(origin, direction, r.Time());
	if(hittable_->Hit(rotated_ray, t_min, t_max, record))
	{
		glm::vec3 p = record.p;
		glm::vec3 normal = record.normal;
		p.x = cos_theta_ * record.p.x + sin_theta_ * record.p.z;
		p.z = -sin_theta_ * record.p.x + cos_theta_ * record.normal.z;
		record.p = p;
		record.normal = normal;
		return true;
	}
	return false;
}