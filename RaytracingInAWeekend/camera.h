#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "ray.h"
class camera 
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 up, float vfov, float aspect_ratio) 
	{
		vec3 u, v, w;
		float theta = vfov * M_PI/ 180.f;
		float half_height = tan(theta / 2.f);
		float half_width = aspect_ratio * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(up, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		
		horizontal = 2 * half_width * u;
		vertical = 2.f * half_height * v;
	}
	
	ray get_ray(float x, float y)
	{
		return ray(origin, lower_left_corner + x * horizontal + y * vertical - origin);
	}
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};