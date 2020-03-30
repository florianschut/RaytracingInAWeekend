#pragma once
#include "material.hpp"
#include "textures.hpp"

class Metal: public Material
{
public:
	Metal(Texture* albedo, float fuzziness) : albedo_(albedo)
	{
		if (fuzziness < 1) fuzz_ = fuzziness; else fuzz_ = 1.f;
	}
	
	~Metal() { delete albedo_; };
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered, float&) const
	{
		glm::vec3 reflected = reflect(normalize(r_in.Direction()),rec.normal);
		scattered = Ray(rec.p, reflected + fuzz_ * utility::RandomInUnitSphere(), r_in.Time());
		attenuation = albedo_->Value(rec.u,rec.v, rec.p);
		return (dot(scattered.Direction(), rec.normal) > 0);
	}

	virtual glm::vec3 Emitted(float, float, const glm::vec3&) const
	{
		return glm::vec3(0.f);
	}
	
	Texture* albedo_;
	float fuzz_;
};
