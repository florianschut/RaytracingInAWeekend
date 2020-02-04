#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>

#include "ray.hpp"

class Camera 
{
public:
	Camera(glm::vec3 look_from, glm::vec3 look_at, glm::vec3 up_vec, float v_fov, float aspect_ratio);
	
	Ray GetRay(float x, float y)
	{
		return Ray(origin_, lower_left_corner_ + x * horizontal_ + y * vertical_ - origin_);
	}

	glm::vec3 GetOrigin() const
	{
		return origin_;
	}
	void SetOrigin(glm::vec3 new_origin);
	
	bool did_change_ = false;

private:
	glm::vec3 origin_;
	glm::vec3 lower_left_corner_;
	glm::vec3 horizontal_;
	glm::vec3 vertical_;
	
	glm::vec3 look_at_;
	glm::vec3 up_;

	float theta_;
	float half_height_;
	float half_width_;
};
