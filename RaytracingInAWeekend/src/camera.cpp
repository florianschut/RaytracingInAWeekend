#include "camera.hpp"

#include <corecrt_math_defines.h>

Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 upVec, float vfov, float aspect_ratio, float aperture, float focus_distance, float time0, float time1):
time0_(time0), time1_(time1)
{
	lens_radius_ = aperture * 0.5f;
	focus_distance_ = focus_distance;
	theta_ = vfov * static_cast<float>(M_PI) / 180.f;
	half_height_ = tan(theta_ / 2.f);
	half_width_ = aspect_ratio * half_height_;
	up_ = upVec;
	origin_ = lookfrom;
	
	LookAt(lookat);
	euler_direction_ = glm::vec3(atan2(w_.x, w_.y),asin(w_.z), 0.f) * 180.f/ static_cast<float>(M_PI);
	SetForward(w_);
}

void Camera::SetOrigin(glm::vec3 new_origin)
{
	origin_ = new_origin;

	lower_left_corner_ = origin_ - half_width_ * focus_distance_* u_ - half_height_ * focus_distance_ * v_ - focus_distance_ * w_;

	did_change_ = true;
}
void Camera::SetEulerDirection(glm::vec3 yaw_pitch_roll)
{
	if (euler_direction_.x == yaw_pitch_roll.x && euler_direction_.y == yaw_pitch_roll.y)
		return;
	euler_direction_ = yaw_pitch_roll;
	glm::vec3 radians = yaw_pitch_roll * static_cast<float>(M_PI) / 180.f;
	SetForward(glm::vec3(cos(radians.x) * cos(radians.y), sin(radians.x) * cos(radians.y), sin(radians.y)));
}

void Camera::SetForward(glm::vec3 dir)
{
	w_ = normalize(dir);
	u_ = normalize(cross(up_, w_));
	v_ = cross(w_, u_);

	lower_left_corner_ = origin_ - half_width_ * focus_distance_ * u_ - half_height_ * focus_distance_ * v_ - focus_distance_ * w_;

	horizontal_ = 2.f * half_width_ * focus_distance_ * u_;
	vertical_ = 2.f * half_height_ * focus_distance_ * v_;

	did_change_ = true;
}

void Camera::LookAt(glm::vec3 tar)
{
	w_ = normalize(origin_ - tar);
}

