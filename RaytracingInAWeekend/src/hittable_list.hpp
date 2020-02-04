#pragma once
#include "hittable.hpp"
class HittableList: public Hittable
{
public:
	HittableList() {}
	HittableList(Hittable** l, int n) : list(l), list_size_(n) { };
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
	
	Hittable** list = nullptr;
	unsigned int list_size_ = 0;
};
