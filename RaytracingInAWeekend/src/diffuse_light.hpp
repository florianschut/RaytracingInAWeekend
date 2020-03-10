#pragma once
#include "material.hpp"
#include "textures.hpp"

class DiffuseLight: public Material
{
public:
	DiffuseLight(Texture* color = new ConstantTexture(glm::vec3(1.f))): color_(color){};

	bool Scatter(const Ray&, const HitRecord&, glm::vec3&, Ray&) const override { return false; };
	virtual glm::vec3 Emitted(float u, float v, const glm::vec3& p) const
	{
		return color_->Value(u, v, p);
	}

private:
	Texture* color_;
};
