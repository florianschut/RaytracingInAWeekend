#pragma once

#include "vec3.h"

class ray
{
public:
	ray() {};
	ray(const vec3& origin, const vec3& direction):A(origin), B(direction){}

	vec3 point_at_parameter(float t) const { return A + B * t; }

	union
	{
		vec3 A;
		vec3 origin;
	};
	
	union
	{
		vec3 B;
		vec3 direction;
	};
};