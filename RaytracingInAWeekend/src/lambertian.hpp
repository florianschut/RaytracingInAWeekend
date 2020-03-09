#pragma once
#include "material.hpp"

#include  <glm/glm.hpp>

#include "random.hpp"
#include "textures.hpp"

class Lambertian :public Material
{
public:
	Lambertian(Texture* albedo) : albedo_(albedo) {}
	~Lambertian() { delete albedo_; };
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
	{
		glm::vec3 target = rec.p + rec.normal + utility::RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, r_in.Time());
		attenuation = albedo_->Value(rec.u, rec.v, rec.p);
		return true;
	}
	
	Texture* albedo_;
};
