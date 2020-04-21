#pragma once
#include <glm/vec3.hpp>

#include "hittable.hpp"
#include "pdf.hpp"
#include "cube_map.hpp"

//Forward declarations
class Ray;
class Camera;
class CubeMap;

class Background
{
public:
	virtual glm::vec3 Value(const Ray&) const {
		return glm::vec3();
	}
};

class World
{
public:
	World(std::shared_ptr<Hittable> hittable, std::shared_ptr<Hittable> lights, std::shared_ptr<Camera> camera, std::shared_ptr<Background> background = std::make_shared<Background>())
		: hittable_(hittable), lights_(lights), camera_(camera), background_(background) {}

	inline bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
	inline std::shared_ptr<HittablePdf> LightPdf(HitRecord& rec) const;

	std::shared_ptr<Camera> GetCamera() { return camera_; }
	glm::vec3 BackgroundColor(const Ray& r) const  { return background_->Value(r); }
	
private:
	std::shared_ptr<Hittable> hittable_;
	std::shared_ptr<Hittable> lights_;
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Background> background_;
};

inline bool World::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
{
	return hittable_->Hit(r, t_min, t_max, record);
}

inline std::shared_ptr<HittablePdf> World::LightPdf(HitRecord& rec) const
{
	return std::make_shared<HittablePdf>(lights_, rec.p);
}


class SkyBackground : public Background
{
	inline glm::vec3 Value(const Ray& r) const override;
};

inline glm::vec3 SkyBackground::Value(const Ray& r) const
{
	const auto t = 0.5f * (normalize(r.Direction()).y + 1.0f);
	return (1.0f - t) * glm::vec3(1.f, 1.f, 1.f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}


class CubeMapBackground: public Background
{
public:
	CubeMapBackground(const char* dir_name) : cube_map_( new CubeMap(dir_name)){}
	~CubeMapBackground() { delete cube_map_; }
	
	inline glm::vec3 Value(const Ray& r) const override;

private:
	CubeMap* cube_map_ = nullptr;
};

glm::vec3 CubeMapBackground::Value(const Ray& r) const
{
	return cube_map_->Value(r.Direction());
}
