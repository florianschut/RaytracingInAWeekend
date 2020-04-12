#pragma once
#include "hittable.hpp"
#include "aabb.hpp"

class Translate: public Hittable
{
public:
	Translate(std::shared_ptr<Hittable> hittable, const glm::vec3& offset): hittable_(hittable), offset_(offset){}
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;
private:
	std::shared_ptr<Hittable> hittable_;
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
	RotateY(std::shared_ptr<Hittable> hittable, float angle);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

private:
	std::shared_ptr<Hittable> hittable_;
	float sin_theta_, cos_theta_;
	bool has_box_;
	AABB box_;
};

inline bool RotateY::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	glm::vec3 origin = r.Origin();
	glm::vec3 direction = r.Direction();

	origin.x = cos_theta_ * r.Origin().x - sin_theta_ * r.Origin().z;
	origin.z = sin_theta_ * r.Origin().x + cos_theta_ * r.Origin().z;

	direction.x = cos_theta_ * r.Direction().x - sin_theta_ * r.Direction().z;
	direction.z = sin_theta_ * r.Direction().x + cos_theta_ * r.Direction().z;

	Ray rotated_ray(origin, direction, r.Time());

	if (!hittable_->Hit(rotated_ray, t_min, t_max, rec))
		return false;

	glm::vec3 p = rec.p;
	glm::vec3 normal = rec.normal;
	p.x = cos_theta_ * rec.p.x + sin_theta_ * rec.p.z;
	p.z = -sin_theta_ * rec.p.x + cos_theta_ * rec.normal.z;
	normal.x = cos_theta_ * rec.normal.x + sin_theta_*rec.normal.z;
	normal.z = -sin_theta_ * rec.normal.x + cos_theta_ * rec.normal.z;

	rec.p = p;
	rec.SetFaceNormal(rotated_ray, normal);
	
	return true;
	
}