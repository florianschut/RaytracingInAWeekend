#pragma once

#include "hittable.hpp"

class MovingSphere : public Hittable
{
public:
	MovingSphere(const glm::vec3 center0, const glm::vec3 center1, const float time0, const float time1, const float radius, std::shared_ptr<Material> material) :
		center0_(center0), center1_(center1),time0_(time0), time1_(time1), radius_(radius), material_(material) {};

	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;
	glm::vec3 Center(float time) const;
private:
	const glm::vec3 center0_, center1_;
	const float time0_, time1_;
	float radius_;
	std::shared_ptr<Material> material_;
};

inline glm::vec3 MovingSphere::Center(float time) const
{
	return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}
