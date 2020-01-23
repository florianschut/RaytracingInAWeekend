#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "ray.h"
class camera 
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 upVec, float vfov, float aspect_ratio) 
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
	vec3 get_origin() const
	{
		return origin;
	}
	void set_origin(vec3 new_origin)
	{
		origin = new_origin;

		vec3 w = unit_vector(origin - look_at);
		vec3 u = unit_vector(cross(up, w));
		vec3 v = cross(w, u);
		lower_left_corner = origin - half_width * u - half_height * v - w;

		horizontal = 2 * half_width * u;
		vertical = 2.f * half_height * v;
	}

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;

	vec3 look_at;
	vec3 up;

	float theta;
	float half_height;
	float half_width;
};