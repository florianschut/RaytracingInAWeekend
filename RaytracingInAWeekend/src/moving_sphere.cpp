#include "moving_sphere.hpp"
#include "aabb.hpp"

bool MovingSphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	const glm::vec3 oc = r.Origin() - Center(r.Time());
	const float a = dot(r.Direction(), r.Direction());
	const float b = dot(oc, r.Direction());
	const float c = dot(oc, oc) - radius_ * radius_;

	const float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float t_temp = (-b - sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - Center(r.Time())) / radius_;
			record.mat_ptr = material_;
			return true;
		}

		t_temp = (-b + sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - Center(r.Time())) / radius_;
			record.mat_ptr = material_;
			return true;
		}
	}
	return false;
}

bool MovingSphere::BoundingBox(float time0, float time1, AABB& box) const
{
	AABB box0(Center(time0) - glm::vec3(radius_), Center(time0) + glm::vec3(radius_));
	AABB box1(Center(time1) - glm::vec3(radius_), Center(time1) + glm::vec3(radius_));
	box = AABB::SurroundingBox(box0, box1);
	return true;
}
