#pragma once

#include "ray.h"
class camera 
{
public:
	camera(float aspect_ratio) : 
		origin(vec3(0)), 
		lower_left_corner(vec3(-aspect_ratio, -1.f, -1.f)), 
		horizontal(vec3(2* aspect_ratio, 0.f, 0.f)), 
		vertical(vec3(0.f, 2.f, 0.f)) 
	{}
	
	ray get_ray(float x, float y)
	{
		return ray(origin, lower_left_corner + x * horizontal + y * vertical - origin);
	}
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};