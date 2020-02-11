#include "moving_sphere.hpp"

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
