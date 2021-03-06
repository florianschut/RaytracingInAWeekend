#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>

#include "ray.hpp"
#include "random.hpp"
class Camera 
{
public:
	Camera(glm::vec3 look_from, glm::vec3 look_at, glm::vec3 up_vec, float v_fov, float aspect_ratio, float aperture, float focus_distance, float time0, float time1);
	
	Ray GetRay(float x, float y) const 
	{
		float time = time0_ + utility::RandomFloat() * (time1_ - time0_);
		if (lens_radius_ == 0.0f)
			return Ray(origin_, lower_left_corner_ + x * horizontal_ + y * vertical_ - origin_, time);
		glm::vec3 ray_dir = lens_radius_ * utility::RandomInUnitDisc();
		glm::vec3 offset = u_ * ray_dir.x + v_ * ray_dir.y;
		return Ray(origin_ + offset, lower_left_corner_ + x * horizontal_ + y * vertical_ - origin_ - offset, time);
	}

	void SetOrigin(glm::vec3 new_origin);

	glm::vec3 GetOrigin() const { return origin_; }

	void SetEulerDirection(glm::vec3 yaw_pitch_roll);

	void SetForward(glm::vec3 dir);

	//Gets the direction of the camera in Yaw-Pitch-Roll components	
	glm::vec3 GetEulerDirection() const { return euler_direction_; }

	void LookAt(glm::vec3 tar);
	
	bool did_change_ = false;

private:
	glm::vec3 origin_;
	glm::vec3 lower_left_corner_;
	glm::vec3 horizontal_;
	glm::vec3 vertical_;
	
	glm::vec3 w_;
	glm::vec3 u_;
	glm::vec3 v_;

	glm::vec3 euler_direction_;

	glm::vec3 up_;

	float theta_;
	float half_height_;
	float half_width_;
	float lens_radius_;
	float focus_distance_;
	float time0_, time1_;
};
