#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:
	Ray(const glm::vec3& origin = glm::vec3(), const glm::vec3& direction = glm::vec3()):a_(origin), b_(direction){}

	glm::vec3 PointAtParameter(float t) const { return a_ + b_ * t; }

	union
	{
		glm::vec3 a_;
		glm::vec3 origin;
	};
	
	union
	{
		glm::vec3 b_;
		glm::vec3 direction;
	};
};
