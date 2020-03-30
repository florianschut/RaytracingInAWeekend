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
	
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered, float& pdf) const
	{
		glm::vec3 target = glm::normalize(rec.normal + utility::RandomUnitVec());
		scattered = Ray(rec.p, target, r_in.Time());
		attenuation = albedo_->Value(rec.u, rec.v, rec.p);
		pdf = glm::dot(rec.normal, scattered.Direction()) / static_cast<float>(M_PI);
		return true;
	}

	float ScatteringPdf(const Ray&, const HitRecord& rec, Ray& scattered) const override
	{
		const auto cosine = glm::dot(rec.normal, normalize(scattered.Direction()));
		return cosine < 0.f ? 0 : cosine / static_cast<float>(M_PI);
	}
	
	Texture* albedo_;
};
