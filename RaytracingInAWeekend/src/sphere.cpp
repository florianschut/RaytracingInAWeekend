#include "sphere.hpp"
#include "aabb.hpp"

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	const auto oc = r.Origin() - center_;
	const auto a = glm::dot(r.Direction(), r.Direction());
	const auto b = dot(oc, r.Direction());
	const auto c = dot(oc, oc) - radius_ * radius_;

	const float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float t_temp = (-b - sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - center_) / radius_;
			record.mat_ptr = material_;
			return true;
		}

		t_temp = (-b + sqrt(discriminant)) / a;
		if (t_temp < t_max && t_temp > t_min)
		{
			record.t = t_temp;
			record.p = r.PointAtParameter(t_temp);
			record.normal = (record.p - center_) / radius_;
			record.mat_ptr = material_;
			return true;
		}
	}
	return false;
}

bool Sphere::BoundingBox(const float, const float, AABB& box) const
{
	box = AABB(center_ - glm::vec3(radius_), center_ + glm::vec3(radius_));
	return true;
}
