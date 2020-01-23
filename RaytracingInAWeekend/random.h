#pragma once

#include <functional>
#include <random>

#include <glm/glm.hpp>

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> random_generator =
		std::bind(distribution, generator);
	return random_generator();
}

inline glm::vec3 random_in_unit_sphere()
{
	glm::vec3 retval;
	do
	{
		retval = 2.f * glm::vec3(random_double(), random_double(), random_double()) - glm::vec3(1.f);
	} while ((length(retval) * length(retval)) >= 1.0f);
	
	return retval;
}
