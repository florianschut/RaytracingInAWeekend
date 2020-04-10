#pragma once
#include "hittable.hpp"
#include "ray.hpp"
#include "pdf.hpp"

struct ScatterRecord
{
	Ray specular_ray;
	bool specular;
	glm::vec3 attenuation;
	std::shared_ptr<Pdf> pdf;
};

class Material
{
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord&) const = 0;
	virtual float ScatteringPdf(const Ray&, const HitRecord&, Ray&) const { return 0.f; }
	virtual glm::vec3 Emitted(const Ray&, const HitRecord&) const { return glm::vec3(0); }
};
