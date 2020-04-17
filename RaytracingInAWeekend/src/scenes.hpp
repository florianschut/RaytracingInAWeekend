#pragma once
#include <memory>

#include <glm/vec3.hpp>

class Hittable;
class Camera;

class Scene
{
public:
	static std::shared_ptr<Hittable> EmptyCornell(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect, glm::vec3 dimensions = glm::vec3(1.f), float light_intensity = 13.f, float light_size = .3f, bool bvh = false, bool inverse_z = false);
	static std::shared_ptr<Hittable> CornellScene(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect);
	static std::shared_ptr<Hittable> CornellBunny(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect);
	static std::shared_ptr<Hittable> CornellDragon(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect);
	static std::shared_ptr<Hittable> CornellTeapot(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect);
	static std::shared_ptr<Hittable> TriangleBoxScene(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect);

	private:
	Scene() = default;
};
