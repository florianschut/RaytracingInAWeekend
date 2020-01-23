#pragma once
#include "material.h"

class metal: public material
{
public:
	metal(const glm::vec3& albedo, float fuzziness) : albedo(albedo)
	{
		if (fuzziness < 1) fuzz = fuzziness; else fuzz = 1.f;
	}
	
	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const
	{
		glm::vec3 reflected = reflect(normalize(r_in.direction),rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction, rec.normal) > 0);
	}
	
	glm::vec3 albedo;
	float fuzz;
};
