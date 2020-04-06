#pragma once

#include <glm/glm.hpp>

class ONB
{
public:
	ONB() = default;
	~ONB() = default;

	glm::vec3 operator[](const int i) const { return axis_[i]; }
	
	glm::vec3 u() const { return axis_[0];}
	glm::vec3 v() const { return axis_[1];}
	glm::vec3 w() const { return axis_[2];}


	glm::vec3 Local(glm::vec3 a) const
	{
		return a.x* u() + a.y * v() + a.z * w();
	}

	void BuildFromW(const glm::vec3& w);
	
private:
	glm::vec3 axis_[3];
};

inline void ONB::BuildFromW(const glm::vec3& w)
{
	axis_[2] = normalize(w);
	glm::vec3 a = (fabs(ONB::w().x) > 0.9f ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0));
	axis_[1] = normalize(cross(ONB::w(), a));
	axis_[0] = cross(ONB::w(), v());
}

