#include "camera.hpp"

#include <corecrt_math_defines.h>

Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio, float aperture, float focus_distance)
{
	lens_radius_ = aperture * 0.5f;
	focus_distance_ = focus_distance;
	theta_ = vfov * static_cast<float>(M_PI) / 180.f;
	half_height_ = tan(theta_ / 2.f);
	half_width_ = aspect_ratio * half_height_;
	look_at_ = lookat;
	up_ = upVec;

	SetOrigin(lookfrom);
}

void Camera::SetOrigin(glm::vec3 new_origin)
{
	origin_ = new_origin;

	w_ = normalize(origin_ - look_at_);
	u_ = normalize(cross(up_, w_));
	v_ = cross(w_, u_);
	lower_left_corner_ = origin_ - half_width_ * u_ - half_height_ * v_ - focus_distance_ * w_;

	horizontal_ = 2.f * half_width_ * focus_distance_ * u_;
	vertical_ = 2.f * half_height_ * focus_distance_ * v_;

	did_change_ = true;
}
