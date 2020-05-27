#pragma once
#include "hittable_list.hpp"
#include "triangle.hpp"
#include "bvh_node.hpp"

struct VertexData
{
	float x, y, z, u, v;
};

class Mesh: public Hittable
{
public:
	Mesh(const char* path, std::shared_ptr<Material> material);
	Mesh(float* vertices, unsigned int stride, unsigned int pos_offset, unsigned int normal_offset, unsigned int triangle_count, std::shared_ptr<Material> material);
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;
	void ImGuiMenu() override;
	
private:
	std::string name_;
	std::shared_ptr<Material> material_;
	std::shared_ptr<HittableList> triangle_list_;
	std::shared_ptr<BvhNode> mesh_bvh;
};

inline bool Mesh::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	return mesh_bvh->Hit(r, t_min, t_max, record);
}

inline bool Mesh::BoundingBox(const float time0, const float time1, AABB& box) const
{
	return triangle_list_->BoundingBox(time0, time1, box);
}
