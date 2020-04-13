#pragma once

#include "material.hpp"

bool Refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
	glm::vec3 uv = normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float Schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Dielectric: public Material
{
public:
	Dielectric(float reflection_index) : ref_index_(reflection_index) {}

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const
	{
		srec.specular = true;
		srec.pdf = nullptr;
		srec.attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		
		float etai_over_etat = rec.front_face ? 1.0f / ref_index_ : ref_index_;

		glm::vec3 unit_direction = normalize(r_in.Direction());
		float cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0f);
		float sin_theta = sqrt(1.0f - cos_theta * cos_theta);
		
		if(etai_over_etat * sin_theta > 1.0f || utility::RandomFloat() < Schlick(cos_theta, etai_over_etat))
		{
			glm::vec3 reflected = reflect(unit_direction, rec.normal);
			srec.specular_ray = Ray(rec.p, reflected, r_in.Time());
			return true;
		}

		glm::vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		srec.specular_ray = Ray(rec.p, refracted, r_in.Time());
		return true;
	}
	
	float ref_index_;
};
