#pragma once
#include <algorithm>
#include <glm/glm.hpp>

class ONB
{
public:
	ONB() = default;
	~ONB() = default;

	glm::vec3 operator[](const int i) const { return axis_[i]; }

	glm::vec3 u() const { return axis_[0]; }
	glm::vec3 v() const { return axis_[1]; }
	glm::vec3 w() const { return axis_[2]; }


	glm::vec3 Local(glm::vec3 a) const
	{
		return a.x * u() + a.y * v() + a.z * w();
	}

	void BuildFromW(const glm::vec3& w);

private:
	glm::vec3 axis_[3];

public:
	
	static float CosTheta(const glm::vec3& w) { return w.z; }
	static float Cos2Theta(const glm::vec3& w) { return w.z * w.z; }
	static float AbsCosTheta(const glm::vec3& w) { return std::abs(w.z); }
	static float Sin2Theta(const glm::vec3& w) { return std::max(0.f, 1.f - Cos2Theta(w)); }
	static float SinTheta(const glm::vec3& w) { return sqrt(Sin2Theta(w)); }
	static float TanTheta(const glm::vec3& w) { return SinTheta(w) / CosTheta(w); }
	static float Tan2Theta(const glm::vec3& w) { return Sin2Theta(w) / Cos2Theta(w); }
	static float CosPhi(const glm::vec3& w)
	{
		float sinTheta = SinTheta(w);
		return (sinTheta == 0) ? 1.f : std::clamp(w.x / sinTheta, -1.f, 1.f);
	}
	static float SinPhi(const glm::vec3& w)
	{
		float sinTheta = SinTheta(w);
		return (sinTheta == 0) ? 0.f : std::clamp(w.y / sinTheta, -1.f, 1.f);
	}
	static float Cos2Phi(const glm::vec3& w) { return CosTheta(w) * CosTheta(w); }
	static float Sin2Phi(const glm::vec3& w) { return SinTheta(w) * SinTheta(w); }
	static float CosDPhi(const glm::vec3& wa, const glm::vec3& wb)
	{
		return std::clamp((wa.x * wb.x + wa.y * wb.y) /
			std::sqrt((wa.x * wa.x + wa.y * wa.y) *
			(wb.x * wb.x + wb.y * wb.y)), -1.f, 1.f);
	}

};

inline void ONB::BuildFromW(const glm::vec3& w)
{
	axis_[2] = normalize(w);
	glm::vec3 a = (fabs(ONB::w().x) > 0.9f ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0));
	axis_[1] = normalize(cross(ONB::w(), a));
	axis_[0] = cross(ONB::w(), v());
}
