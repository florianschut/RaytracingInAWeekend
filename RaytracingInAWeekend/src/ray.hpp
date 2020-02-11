#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:
	Ray(const glm::vec3& origin = glm::vec3(), const glm::vec3& direction = glm::vec3(), float time = 0.0f):a_(origin), b_(direction), time_(time){}

	glm::vec3 PointAtParameter(float t) const { return a_ + b_ * t; }
	glm::vec3 Origin() const { return a_; }
	glm::vec3 Direction() const { return b_; }
	float Time() const { return time_; }
private:
	glm::vec3 a_;
	glm::vec3 b_;

	float time_ = 0.f;
};
