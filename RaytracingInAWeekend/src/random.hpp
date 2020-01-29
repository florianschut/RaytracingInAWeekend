#pragma once

#include <functional>
#include <random>

#include <glm/glm.hpp>

inline float random_float()
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<float()> random_generator =
		std::bind(distribution, generator);
	return random_generator();
}

inline glm::vec3 random_in_unit_sphere()
{
	glm::vec3 retval;
	do
	{
		retval = 2.f * glm::vec3(random_float(), random_float(), random_float()) - glm::vec3(1.f);
	} while ((length(retval) * length(retval)) >= 1.0f);
	
	return retval;
}
