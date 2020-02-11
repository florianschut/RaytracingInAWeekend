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

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 outward_normal;
		glm::vec3 reflected = reflect(r_in.Direction(),rec.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 refracted;

		float reflect_prob;
		float cosine;

		if(dot(r_in.Direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_index_;
			cosine = ref_index_ * dot(r_in.Direction(), rec.normal) / r_in.Direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_index_;
			cosine = -dot(r_in.Direction(), rec.normal) / r_in.Direction().length();
		}

		if (Refract(r_in.Direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = Schlick(cosine, ref_index_);
		}
		else 
		{
			reflect_prob = 1.0f;
		}

		if(utility::RandomFloat() < reflect_prob)
		{
			scattered = Ray(rec.p, reflected, r_in.Time());
		}
		else
		{
			scattered = Ray(rec.p, refracted, r_in.Time());
		}

		return true;
	}
	
	float ref_index_;
};