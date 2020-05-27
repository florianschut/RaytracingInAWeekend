#pragma once
#include <algorithm>

#include "hittable.hpp"
#include "aabb.hpp"

class Triangle: public Hittable
{
public:
	//TODO: no inline plox
	inline Triangle(glm::vec3 vertices[3], std::shared_ptr<Material> material);
	inline Triangle(float* vertices, unsigned int pos_offset, unsigned int normal_offset, unsigned int stride, std::shared_ptr<Material> material);
	inline bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float, const float, AABB& box) const override;
	void ImGuiMenu() override;
private:
	glm::vec3 ver0_;
	glm::vec3 edge0_, edge1_, normal_;;
	std::shared_ptr<Material> material_;
	AABB box_;
	const float epsilon_ = 0.0000001f;
};

Triangle::Triangle(glm::vec3 vertices[3], std::shared_ptr<Material> material)
: ver0_(vertices[0]), material_(material), edge0_(vertices[1] - vertices[0]), edge1_(vertices[2]- vertices[0]), normal_(-normalize(cross(edge1_, edge0_))),
	box_(AABB(glm::vec3(
		std::min(std::min(vertices[0].x, vertices[1].x), vertices[2].x),
		std::min(std::min(vertices[0].y, vertices[1].y), vertices[2].y),
		std::min(std::min(vertices[0].z, vertices[1].z), vertices[2].z)
		), glm::vec3(
			std::max(std::max(vertices[0].x, vertices[1].x), vertices[2].x),
			std::max(std::max(vertices[0].y, vertices[1].y), vertices[2].y),
			std::max(std::max(vertices[0].z, vertices[1].z), vertices[2].z)
			)))
{}

inline Triangle::Triangle(float* vertices, unsigned int pos_offset, unsigned int normal_offset, unsigned int stride, std::shared_ptr<Material> material)
{
	material_= material;
	ver0_ = glm::vec3(vertices[pos_offset], vertices[pos_offset + 1], vertices[pos_offset + 2]);
	edge0_ = glm::vec3(vertices[stride + pos_offset], vertices[stride + pos_offset + 1], vertices[stride + pos_offset + 2]) - ver0_;
	edge1_ = glm::vec3(vertices[2 * stride + pos_offset], vertices[2 * stride + pos_offset + 1], vertices[2 * stride + pos_offset + 2]) - ver0_;
	normal_ = glm::vec3(vertices[normal_offset], vertices[normal_offset + 1], vertices[normal_offset + 2]);
	box_ = AABB(glm::vec3(
		std::min(std::min(ver0_.x, vertices[stride + pos_offset]), vertices[2 * stride + pos_offset]),
		std::min(std::min(ver0_.y, vertices[2 * stride + pos_offset + 1]), vertices[2 * stride + pos_offset + 1]),
		std::min(std::min(ver0_.z, vertices[stride + pos_offset + 2]), vertices[2 * stride + pos_offset + 2])), 
		glm::vec3(
		std::max(std::max(ver0_.x, vertices[stride + pos_offset]), vertices[2 * stride + pos_offset]),
		std::max(std::max(ver0_.y, vertices[2 * stride + pos_offset + 1]), vertices[2 * stride + pos_offset + 1]),
		std::max(std::max(ver0_.z, vertices[stride + pos_offset + 2]), vertices[2 * stride + pos_offset + 2])
		));
}


bool Triangle::Hit(const Ray& r, float, float, HitRecord& record) const
{
	//Adapted from Jacco Bikker's adaption from Moller-Trumbore
	//TODO: Experiment with normal
	if (!(dot(normalize(r.Direction()), normal_) < 0.f))
	{
		return false;//Back Face
	}
		glm::vec3 half = glm::cross(r.Direction(), edge1_);
	const float a = dot(edge0_, half);
	if (a > -epsilon_ && a < epsilon_) return false; //Ray parallel to triangle

	const float f = 1.0f / a;
	const glm::vec3 s = r.Origin() - ver0_;
	const float u = f * dot(s, half);
	if (u < 0.0f || u > 1.0f) return false;//miss
	const glm::vec3 q = cross(s, edge0_);
	const float v = f * dot(r.Direction(), q);
	if (v < 0.0f || v + u > 1.0f) return false;//miss
	const float t = f * dot(edge1_, q);
	if(t > epsilon_ && t < 1/epsilon_)//Intersection!
	{
		record.p = r.PointAtParameter(t);
		record.u = u;
		record.v = v;
		record.t = t;
		record.mat_ptr = material_;
		record.SetFaceNormal(r, -normal_);
		return true;
	}
	return false;
}

inline bool Triangle::BoundingBox(const float, const float, AABB& box) const
{
	box = box_;
	return true;
}
