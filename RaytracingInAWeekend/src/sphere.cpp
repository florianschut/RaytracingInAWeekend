#include "sphere.hpp"

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	const glm::vec3 oc = r.origin - center_;
	const float a = dot(r.direction, r.direction);
	const float b = dot(oc, r.direction);
	const float c = dot(oc, oc) - radius_ * radius_;

	const float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float t_temp = (-b - sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - center_) / radius_;
			record.mat_ptr = mat_ptr;
			return true;
		}

		t_temp = (-b + sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - center_) / radius_;
			record.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}