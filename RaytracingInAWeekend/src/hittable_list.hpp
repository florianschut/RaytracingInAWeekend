#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "hittable.hpp"
#include "random.hpp"

class HittableList: public Hittable
{
public:
	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }
	
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override;

	void Clear() { objects_.clear(); }
	void Add(std::shared_ptr<Hittable> object) { objects_.push_back(object); }

	float PdfValue(const glm::vec3& o, const glm::vec3& v) const override
	{
		auto weight = 1.0f / objects_.size();
		auto sum = 0.0f;

		for (const auto& object : objects_)
			sum += weight * object->PdfValue(o, v);
		return sum;
	}

	glm::vec3 Random(const glm::vec3& o) const override
	{
		auto int_size = static_cast<int>(objects_.size());
		auto random_int = static_cast<int>(utility::RandomFloat() * static_cast<float>(int_size));
		if (random_int >= int_size || random_int < 0)
			std::cerr << "Random int: "<< random_int <<" IntSize: "<< int_size<<std::endl;
		return objects_[random_int]->Random(o);
	}

	std::vector<std::shared_ptr<Hittable>> objects_ ;
};
