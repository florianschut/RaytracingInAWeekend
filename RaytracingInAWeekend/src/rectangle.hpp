#pragma once
#include "hittable.hpp"

#include "aabb.hpp"

class XYRect: public Hittable
{
public:
	XYRect(float x0, float x1, float y0, float y1, float k, Material* mat):
	x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(mat){}
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float, const float, AABB& box) const override
	{
		box = AABB(glm::vec3(x0_, y0_, k_ - 0.0001f), glm::vec3(x1_, y1_, k_ + 0.0001f));
		return true;
	};

private:
	Material* material_;
	float x0_, x1_, y0_, y1_, k_;
};

bool XYRect::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	float t = (k_ - r.Origin().z) / r.Direction().z;
	if (t < t_min || t > t_max)
		return false;
	float x = r.Origin().x + t * r.Direction().x;
	float y = r.Origin().y + t * r.Direction().y;
	if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
		return false;
	record.u = (x - x0_) / (x1_ - x0_);
	record.v = (y - y0_) / (y1_ - y0_);
	record.t = t;
	record.mat_ptr = material_;
	record.p = r.PointAtParameter(t);
	record.normal = glm::vec3(0, 0, 1);
	return true;
}

class XZRect: public Hittable
{
public:
	XZRect(float x0, float x1, float z0, float z1, float k, Material* mat) :
		x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), material_(mat) {}
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float, const float, AABB& box) const override
	{
		box = AABB(glm::vec3(x0_, k_ - 0.0001f, z0_), glm::vec3(x1_, k_ + 0.0001f, z1_));
		return true;
	};

private:
	Material* material_;
	float x0_, x1_, z0_, z1_, k_;
};

bool XZRect::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	float t = (k_ - r.Origin().y) / r.Direction().y;
	if (t < t_min || t > t_max)
		return false;
	float x = r.Origin().x + t * r.Direction().x;
	float z = r.Origin().z + t * r.Direction().z;
	if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
		return false;
	record.u = (x - x0_) / (x1_ - x0_);
	record.v = (z - z0_) / (z1_ - z0_);
	record.t = t;
	record.mat_ptr = material_;
	record.p = r.PointAtParameter(t);
	record.normal = glm::vec3(0, 1, 0);
	return true;
}


class YZRect: public Hittable
{
public:
	YZRect(float y0, float y1, float z0, float z1, float k, Material* mat) :
		y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), material_(mat) {}
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float, const float, AABB& box) const override
	{
		box = AABB(glm::vec3(k_ - 0.0001f, y0_, z0_), glm::vec3(k_ + 0.0001f, y1_, z1_));
		return true;
	};

private:
	Material* material_;
	float y0_, y1_, z0_, z1_, k_;
};

bool YZRect::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	float t = (k_ - r.Origin().x) / r.Direction().x;
	if (t < t_min || t > t_max)
		return false;
	float y = r.Origin().y + t * r.Direction().y;
	float z = r.Origin().z + t * r.Direction().z;
	if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
		return false;
	record.u = (y - y0_) / (y1_ - y0_);
	record.v = (z - z0_) / (z1_ - z0_);
	record.t = t;
	record.mat_ptr = material_;
	record.p = r.PointAtParameter(t);
	record.normal = glm::vec3(1, 0, 0);
	return true;
}

class FlipNormals : public Hittable
{
public:
	FlipNormals(Hittable* p) : hittable_(p) {}

	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override
	{
		if (hittable_->Hit(r, t_min, t_max, record))
		{
			record.normal = -record.normal;
			return true;
		}
		return false;
	}

	bool BoundingBox(const float time0, const float time1, AABB& box) const override
	{
		return hittable_->BoundingBox(time0, time1, box);
	}

private:
	Hittable* hittable_;
};
