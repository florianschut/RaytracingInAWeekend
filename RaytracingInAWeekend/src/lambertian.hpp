#pragma once
#include "material.hpp"

#include  <glm/glm.hpp>

#include "random.hpp"
#include "textures.hpp"
#include "ortho_normal_base.hpp"

class Lambertian :public Material
{
public:
	Lambertian(std::shared_ptr<Texture> albedo) : albedo_(albedo) {}
	~Lambertian() = default;
	
	virtual bool Scatter(const Ray& , const HitRecord& rec, ScatterRecord& srec) const
	{
		srec.specular = false;
		srec.attenuation = albedo_->Value(rec.u, rec.v, rec.p);
		srec.pdf = std::make_shared<CosinePdf>(rec.normal);
		return true;
	}

	float ScatteringPdf(const Ray&, const HitRecord& rec, Ray& scattered) const override
	{
		const auto cosine = glm::dot(rec.normal, normalize(scattered.Direction()));
		return cosine < 0.f ? 0 : cosine / static_cast<float>(M_PI);
	}
	
	std::shared_ptr<Texture> albedo_;
};
