#pragma once

#include <glm/vec3.hpp>

class ray
{
public:
	ray(const glm::vec3& origin = glm::vec3(), const glm::vec3& direction = glm::vec3()):A(origin), B(direction){}

	glm::vec3 point_at_parameter(float t) const { return A + B * t; }

	union
	{
		glm::vec3 A;
		glm::vec3 origin;
	};
	
	union
	{
		glm::vec3 B;
		glm::vec3 direction;
	};
};
