#pragma once
#include "material.hpp"
#include "textures.hpp"

class Isotropic : public Material
{
public:
	Isotropic(std::shared_ptr<Texture> albedo): albedo_(albedo){}

	bool Scatter(const Ray&, const HitRecord& rec, ScatterRecord& srec) const
	{
		srec.specular_ray = Ray(rec.p, utility::RandomInUnitSphere());
		srec.attenuation = albedo_->Value(rec.u, rec.v, rec.p);
		return true;
	}
	
private:
	std::shared_ptr<Texture> albedo_;
};
