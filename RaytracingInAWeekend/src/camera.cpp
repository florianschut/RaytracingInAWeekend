#include "camera.hpp"

#include <corecrt_math_defines.h>

camera::camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio)
{
	theta_ = vfov * static_cast<float>(M_PI) / 180.f;
	half_height_ = tan(theta_ / 2.f);
	half_width_ = aspect_ratio * half_height_;
	look_at_ = lookat;
	up_ = upVec;

	set_origin(lookfrom);
}

void camera::set_origin(glm::vec3 new_origin)
{
	origin_ = new_origin;

	glm::vec3 w = normalize(origin_ - look_at_);
	glm::vec3 u = normalize(cross(up_, w));
	glm::vec3 v = cross(w, u);
	lower_left_corner_ = origin_ - half_width_ * u - half_height_ * v - w;

	horizontal_ = 2.f * half_width_ * u;
	vertical_ = 2.f * half_height_ * v;
}
