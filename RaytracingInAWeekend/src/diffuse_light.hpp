#pragma once
#include "material.hpp"
#include "textures.hpp"

class DiffuseLight: public Material
{
public:
	DiffuseLight(Texture* color = new ConstantTexture(glm::vec3(1.f))): color_(color){};

	bool Scatter(const Ray&, const HitRecord&, ScatterRecord&) const override { return false; }
	virtual glm::vec3 Emitted(const Ray&, const HitRecord& rec) const
	{
		if(rec.front_face)
			return color_->Value(rec.u, rec.v, rec.p);
		return glm::vec3(0);
	}

private:
	Texture* color_;
};
