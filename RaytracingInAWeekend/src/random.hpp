#pragma once

#include <functional>
#include <random>

#include <glm/glm.hpp>

namespace utility
{

	inline float RandomFloat()
	{
		static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		static std::mt19937 generator;
		static std::function<float()> random_generator =
			std::bind(distribution, generator);
		return random_generator();
	}

	inline glm::vec3 RandomInUnitSphere()
	{
		glm::vec3 retval;
		do
		{
			retval = 2.f * glm::vec3(RandomFloat(), RandomFloat(), RandomFloat()) - glm::vec3(1.f);
		} while ((length(retval) * length(retval)) >= 1.0f);

		return retval;
	}

	inline glm::vec3 RandomInUnitDisc()
	{
		glm::vec3 retval;
		do
		{
			retval = 2.0f * glm::vec3(RandomFloat(), RandomFloat(), 0.f) - glm::vec3(1.f, 1.f, 0.f);
		} while (glm::dot(retval, retval) >= 1.0);
		return retval;
	}
}
