#pragma once
#include <memory>

#include <glm/vec3.hpp>

class World;
class Camera;
class Hittable;
class HittableList;

class Scene
{
public:
	static std::shared_ptr<World> CornellScene(float aspect);
	static std::shared_ptr<World> SkyboxScene(float aspect);
	static std::shared_ptr<World> CornellBunny(float aspect);
	static std::shared_ptr<World> CornellDragon(float aspect);
	static std::shared_ptr<World> CornellTeapot(float aspect);
	static std::shared_ptr<World> TriangleBoxScene(float aspect);
	
	private:
	static std::shared_ptr<HittableList> EmptyCornell(std::shared_ptr<Camera>* camera, std::shared_ptr<Hittable>* lights, float aspect, glm::vec3 dimensions = glm::vec3(1.f), float light_intensity = 13.f, float light_size = .3f, bool inverse_z = false);
	Scene() = default;
};
