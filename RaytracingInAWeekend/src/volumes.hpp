#pragma once
#include "hittable.hpp"
#include "textures.hpp"
#include "isotropic.hpp"

class ConstantMedium: public Hittable
{
public:
	ConstantMedium(Hittable* boundry, float density, Texture* albedo): boundry_(boundry), density_(density), phase_function_(new Isotropic(albedo)){};
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	bool BoundingBox(const float time0, const float time1, AABB& box) const override
	{
		return boundry_->BoundingBox(time0, time1, box);
	}
	
private:
	Hittable* boundry_;
	float density_;
	Material* phase_function_;
};

inline bool ConstantMedium::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	// Print occasional samples when debugging. To enable, set enableDebug true.
	const bool enableDebug = false;
	bool debugging = enableDebug && utility::RandomFloat() < 0.00001;
	HitRecord rec1, rec2;

	if (boundry_->Hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if(boundry_->Hit(r, rec1.t+0.0001f, FLT_MAX, rec2))
		{
			if (rec1.t < t_min)
				rec1.t = t_min;

			if (rec2.t > t_max)
				rec2.t = t_max;
			
			if (rec1.t >= rec2.t)
				return false;

			if (rec1.t < 0)
				rec1.t = 0;

			float distance_inside_boundry = (rec2.t - rec1.t) * length(r.Direction());
			float hit_distance = -(1.f / density_) * log(utility::RandomFloat());

			if (hit_distance < distance_inside_boundry)
			{
				record.t = rec1.t + hit_distance / length(r.Direction());
				record.p = r.PointAtParameter(record.t);

				if (debugging) {
					std::cerr << "hit_distance = " << hit_distance << '\n'
						<< "rec.t = " << record.t << '\n'
						<< "rec.p = " << record.p.x << " "<< record.p.y <<" "<< record.p.z << '\n'
					<< "direction.length = " << length(r.Direction()) << '\n';
				}
				
				record.normal = glm::vec3(1, 0, 0);
				record.mat_ptr = phase_function_;
				return true;
			}
		}
	}
	return false;
}