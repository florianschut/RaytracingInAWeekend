#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>

#include "ray.h"

class camera 
{
public:
	camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio)
	{
		theta = vfov * M_PI/ 180.f;
		half_height = tan(theta / 2.f);
		half_width = aspect_ratio * half_height;
		look_at = lookat;
		up = upVec;
		
		set_origin(lookfrom);
	}
	
	ray get_ray(float x, float y)
	{
		return ray(origin, lower_left_corner + x * horizontal + y * vertical - origin);
	}

	glm::vec3 get_origin() const
	{
		return origin;
	}
	void set_origin(glm::vec3 new_origin)
	{
		origin = new_origin;

		glm::vec3 w = normalize(origin - look_at);
		glm::vec3 u = normalize(cross(up, w));
		glm::vec3 v = cross(w, u);
		lower_left_corner = origin - half_width * u - half_height * v - w;

		horizontal = 2 * half_width * u;
		vertical = 2.f * half_height * v;
	}
	
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
