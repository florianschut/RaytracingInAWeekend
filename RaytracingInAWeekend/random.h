#pragma once

#include <functional>
#include <random>

#include "vec3.h"

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> random_generator =
		std::bind(distribution, generator);
	return random_generator();
}

inline vec3 random_in_unit_sphere()
{
	vec3 retval;
	do
	{
		retval = 2 * vec3(random_double(), random_double(), random_double()) - vec3(1.f);
	} while (retval.squared_length() >= 1.0f);
	return retval;
}
