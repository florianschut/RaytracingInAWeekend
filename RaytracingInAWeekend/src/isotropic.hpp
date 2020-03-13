#pragma once
#include "material.hpp"
#include "textures.hpp"

class Isotropic : public Material
{
public:
	Isotropic(Texture* albedo): albedo_(albedo){}

	bool Scatter(const Ray&, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
	{
		scattered = Ray(rec.p, utility::RandomInUnitSphere());
		attenuation = albedo_->Value(rec.u, rec.v, rec.p);
		return true;
	}
	
private:
	Texture* albedo_;
};
