#pragma once
#include <corecrt_math_defines.h>


#include "hittable.hpp"
#include "ortho_normal_base.hpp"
#include "random.hpp"

class Sphere: public Hittable
{
public:
	Sphere(const glm::vec3 center, const float radius, std::shared_ptr<Material> material): center_(center), radius_(radius), material_(material){};
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

	float PdfValue(const glm::vec3& o, const glm::vec3& v) const override
	{
		HitRecord rec;
		if (!this->Hit(Ray(o, v), 0.001f, FLT_MAX, rec))
			return 0.f;
		
		auto cos_theta_max= sqrt(1 - radius_ * radius_ / ((length(center_ - o) * length(center_ - o))));
		auto solid_angle = 2 * static_cast<float>(M_PI) * (1 - cos_theta_max);

		return 1 / solid_angle;
	}

	glm::vec3 Random(const glm::vec3& o) const override
	{
		glm::vec3 direction = center_ - o;
		auto distance_squared = length(direction) * length(direction);
		ONB uvw;
		uvw.BuildFromW(direction);
		return uvw.Local(utility::RandomToSphere(radius_, distance_squared));
	}
	
	static void UV(const glm::vec3& p, float& u, float& v);
private:
	glm::vec3 center_;
	float radius_;
	std::shared_ptr<Material> material_;
};
