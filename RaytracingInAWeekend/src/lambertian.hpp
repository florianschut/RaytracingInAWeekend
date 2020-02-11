#pragma once
#include "material.hpp"

#include  <glm/glm.hpp>
#include "random.hpp"

class Lambertian :public Material
{
public:
	Lambertian(const glm::vec3& albedo_) : albedo_(albedo_) {}

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 target = rec.p + rec.normal + utility::RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, r_in.Time());
		attenuation = albedo_;
		return true;
	}
	
	glm::vec3 albedo_;
};
