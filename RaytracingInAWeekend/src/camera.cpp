#include "camera.h"

#include <corecrt_math_defines.h>

camera::camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio)
{
	theta = vfov * static_cast<float>(M_PI) / 180.f;
	half_height = tan(theta / 2.f);
	half_width = aspect_ratio * half_height;
	look_at = lookat;
	up = upVec;

	set_origin(lookfrom);
}

void camera::set_origin(glm::vec3 new_origin)
{
	origin = new_origin;

	glm::vec3 w = normalize(origin - look_at);
	glm::vec3 u = normalize(cross(up, w));
	glm::vec3 v = cross(w, u);
	lower_left_corner = origin - half_width * u - half_height * v - w;

	horizontal = 2.f * half_width * u;
	vertical = 2.f * half_height * v;
}
