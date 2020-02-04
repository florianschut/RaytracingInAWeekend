#pragma once
#include "material.hpp"

class Metal: public Material
{
public:
	Metal(const glm::vec3& albedo, float fuzziness) : albedo_(albedo)
	{
		if (fuzziness < 1) fuzz_ = fuzziness; else fuzz_ = 1.f;
	}
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 reflected = reflect(normalize(r_in.direction),rec.normal);
		scattered = Ray(rec.p, reflected + fuzz_ * utility::RandomInUnitSphere());
		attenuation = albedo_;
		return (dot(scattered.direction, rec.normal) > 0);
	}
	
	glm::vec3 albedo_;
	float fuzz_;
};
