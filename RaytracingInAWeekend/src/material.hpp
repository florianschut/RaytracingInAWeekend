#pragma once
#include "hittable.hpp"
#include "ray.hpp"
#include "pdf.hpp"

struct ScatterRecord
{
	Ray specular_ray;
	bool specular = false;
	glm::vec3 attenuation = glm::vec3(0.f);
	std::shared_ptr<Pdf> pdf = nullptr;
};

class Material
{
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord&) const = 0;
	virtual float ScatteringPdf(const Ray&, const HitRecord&, Ray&) const { return 0.f; }
	virtual glm::vec3 Emitted(const Ray&, const HitRecord&) const { return glm::vec3(0); }
};
