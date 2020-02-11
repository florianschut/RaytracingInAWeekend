#pragma once

#include "hittable.hpp"

class MovingSphere : public Hittable
{
public:
	MovingSphere(const glm::vec3 center0, const glm::vec3 center1, const float time0, const float time1, const float radius, Material* material) :
		center0_(center0), center1_(center1),time0_(time0), time1_(time1), radius_(radius), material_(material) {};

	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
	//virtual bool BoundingBox(float time0, float time1, Aabb& box) const;
	glm::vec3 Center(float time) const;
private:
	const glm::vec3 center0_, center1_;
	const float time0_, time1_;
	float radius_;
	Material* material_;
};

inline glm::vec3 MovingSphere::Center(float time) const
{
	return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}
