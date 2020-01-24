#pragma once

#include "material.h"

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
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

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric: public material
{
public:
	dielectric(float reflection_index) : ref_index(reflection_index) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const
	{
		glm::vec3 outward_normal;
		glm::vec3 reflected = reflect(r_in.direction,rec.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 refracted;

		float reflect_prob;
		float cosine;

		if(dot(r_in.direction, rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_index;
			cosine = ref_index * dot(r_in.direction, rec.normal) / r_in.direction.length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_index;
			cosine = -dot(r_in.direction, rec.normal) / r_in.direction.length();
		}

		if (refract(r_in.direction, outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_index);
		}
		else 
		{
			reflect_prob = 1.0f;
		}

		if(random_float() < reflect_prob)
		{
			scattered = ray(rec.p, reflected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}

		return true;
	}
	
	float ref_index;
};