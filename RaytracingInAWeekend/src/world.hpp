#pragma once
#include "camera.hpp"
#include "hittable.hpp"
#include "pdf.hpp"
#include <iostream>

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
	World(std::shared_ptr<Hittable> hittable, std::shared_ptr<Hittable> lights, std::shared_ptr<Camera> camera, std::shared_ptr<Background> background = std::make_shared<Background>()): hittable_(hittable), lights_(lights), camera_(camera), background_(background){};
	bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const { return hittable_->Hit(r, t_min, t_max, record); }
	std::shared_ptr<HittablePdf> LightPdf(HitRecord& rec) const { return std::make_shared<HittablePdf>(lights_, rec.p); }
	std::shared_ptr<const Camera> GetCamera() const  { return camera_; }
	glm::vec3 BackgroundColor(const Ray& r) { return background_->Value(r); }
private:
	std::shared_ptr<Hittable> lights_;
	std::shared_ptr<Hittable> hittable_;
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Background> background_;
};

class SkyBackground : public Background
{
	glm::vec3 Value(const Ray& r) const override
	{
		const auto t = 0.5f * (normalize(r.Direction()).y + 1.0f);
		return (1.0f - t) * glm::vec3(1.f, 1.f, 1.f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
};
