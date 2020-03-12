#include "instance.hpp"

#include "aabb.hpp"
#include <corecrt_math_defines.h>

bool Translate::BoundingBox(const float time0, const float time1, AABB& box) const
{
	if(hittable_->BoundingBox(time0, time1, box))
	{
		box = AABB(box.Min() + offset_, box.Max() + offset_);
		return true;
	}
	return false;
}

RotateY::RotateY(Hittable* hittable, float angle): hittable_(hittable)
{
	float radians = static_cast<float>(M_PI / 180.0)* angle;
	sin_theta_ = sin(radians);
	cos_theta_ = cos(radians);
	has_box_ = hittable->BoundingBox(0, 1, box_);
	glm::vec3 min(FLT_MAX);
	glm::vec3 max(-FLT_MAX);
	for (int i = 0; i< 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				float x = i * box_.Max().x + (1 - i) * box_.Min().x;
				float y = j * box_.Max().y + (1 - j) * box_.Min().y;
				float z = k * box_.Max().z + (1 - k) * box_.Min().z;
				float newx = cos_theta_ * x + sin_theta_ * z;
				float newz = -sin_theta_ * x + cos_theta_ * z;
				glm::vec3 tester(newx, y, newz);
				for (int c = 0; c< 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	box_ = AABB(min, max);
}



bool RotateY::BoundingBox(const float,  const float, AABB& box) const
{
	box = box_;
	return has_box_;
}
