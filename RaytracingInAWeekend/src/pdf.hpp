#pragma once
#include <glm/glm.hpp>

#include "random.hpp"
#include "ortho_normal_base.hpp"
//#include "hittable.hpp"


class Pdf
{
public:
	virtual float Value(const glm::vec3& direction) const = 0;
	virtual glm::vec3 Generate() const = 0;
};

class CosinePdf : public Pdf
{
public:
	CosinePdf(const glm::vec3& w) { uvw_.BuildFromW(w); }

	float Value(const glm::vec3& direction) const override
	{
		const auto cosine = dot(normalize(direction), uvw_.w());
		return (cosine <= 0.f) ? 0 : cosine / static_cast<float>(M_PI);
	}

	glm::vec3 Generate() const override
	{
		return uvw_.Local(utility::RandomCosineDirection());
	}
	
private:
	ONB uvw_;
};

class HittablePdf: public Pdf
{
public:
	HittablePdf(std::shared_ptr<Hittable> hittable, glm::vec3& origin) : origin_(origin), hittable_(hittable) {}

	float Value(const glm::vec3& direction) const override
	{
		return hittable_->PdfValue(origin_, direction);
	}

	glm::vec3 Generate() const override
	{
		return hittable_->Random(origin_);
	}
	
private:
	glm::vec3 origin_;
	std::shared_ptr<Hittable> hittable_;
};

class MixturePdf: public Pdf
{
public:
	MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) { p[0] = p0; p[1] = p1; }

	float Value(const glm::vec3& direction) const override
	{
		return 0.5f * p[0]->Value(direction) + 0.5f * p[1]->Value(direction);
	}

	glm::vec3 Generate() const override
	{
		if (utility::RandomFloat() < 0.5)
			return p[0]->Generate();
		return p[1]->Generate();
	}

private:
	std::shared_ptr<Pdf> p[2];
};
