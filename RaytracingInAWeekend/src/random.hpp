#pragma once

#include <functional>
#include <random>

#include <glm/glm.hpp>
#include <corecrt_math_defines.h>

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

	inline float RandomFloat(float min, float max)
	{
		return min + (max - min) * RandomFloat();
	}

	inline glm::vec3 RandomVec(float min, float max)
	{
		return glm::vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
	}

	inline glm::vec3 RandomUnitVec()
	{
		const auto a = RandomFloat(0.f, 2.f * static_cast<float>(M_PI));
		const auto z = RandomFloat(-1.f, 1.f);
		const auto r = sqrt(1.f - z * z);
		return glm::vec3(r * cos(a), r * sin(a), z);
	}

	inline glm::vec3 RandomCosineDirection()
	{
		auto r1 = RandomFloat();
		auto r2 = RandomFloat();
		auto z = sqrt(1 - r2);

		auto phi = 2 * static_cast<float>(M_PI) * r1;
		auto x = cos(phi) * sqrt(r2);
		auto y = sin(phi) * sqrt(r2);
		return glm::vec3(x, y, z);

	}

	inline glm::vec3 RandomInUnitSphere()
	{
		glm::vec3 retval;
		do
		{
			retval = RandomVec(-1.f, 1.f);
		} while ((length(retval) * length(retval)) >= 1.0f);

		return retval;
	}

	inline glm::vec3 RandomInHemisphere(const glm::vec3& normal)
	{
		glm::vec3 random = RandomInUnitSphere();
		if (glm::dot(normal, random) > 0.f)
			return random;
		return -random;
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
