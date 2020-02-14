#pragma once
#include <glm/vec3.hpp>

#include "ray.hpp"

class AABB
{
public:
	AABB():min_(glm::vec3()), max_(glm::vec3()){};
	AABB(const glm::vec3& a, const glm::vec3 b):min_(a), max_(b){};

	glm::vec3 Min() const { return min_; }
	glm::vec3 Max() const { return max_; }

	static AABB SurroundingBox(AABB box0, AABB box1);
	
	bool Hit( const Ray& ray, float tmin, float tmax) const
	{
		for (auto a = 0; a < 3; a++)
		{
			float inv_dir = 1.0f / ray.Direction()[a];
			auto t0 = (min_[a] - ray.Direction()[a]) * inv_dir;
			auto t1 = (max_[a] - ray.Direction()[a]) * inv_dir;
			if (inv_dir < 0.f)
				std::swap(t0, t1);
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
private:
	glm::vec3 min_, max_;
};