#pragma once
#include "ray.hpp"
#include <memory>
#include <string>

#include <glm/glm.hpp>

class AABB;
class Material;
struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	bool front_face;
	float u, v;
	std::shared_ptr<Material> mat_ptr = nullptr;

	void SetFaceNormal(const Ray& r, const glm::vec3& outward_normal)
	{
		front_face = dot(r.Direction(), outward_normal) < 0.0f;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
	virtual bool BoundingBox(const float time0, const float time1, AABB& box) const = 0;
	//TODO: Should probably be pure virtual and have source file removed
	virtual void ImGuiMenu();
	virtual float PdfValue(const glm::vec3&, const glm::vec3&) const
	{
		return 0.f;
	}

	virtual glm::vec3 Random(const glm::vec3&) const
	{
		return glm::vec3(1, 0, 0);
	}

	std::string name_ = "";
};
