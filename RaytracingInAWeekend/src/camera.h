#pragma once
#include <glm/glm.hpp>

#include "ray.h"

class camera 
{
public:
	camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio);
	
	ray get_ray(float x, float y)
	{
		return ray(origin, lower_left_corner + x * horizontal + y * vertical - origin);
	}

	glm::vec3 get_origin() const
	{
		return origin;
	}
	
	void set_origin(glm::vec3 new_origin);
	
private:
	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	
	glm::vec3 look_at;
	glm::vec3 up;
	
	float theta;
	float half_height;
	float half_width;
};
