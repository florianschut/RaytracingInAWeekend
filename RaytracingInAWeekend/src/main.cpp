#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "renderer.hpp"
#include "random.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "metal.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"

Hittable* random_scene()
{
	const int n = 500;
	auto list = new Hittable * [n + 1];
	list[0] = new Sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			const float choose_mat = utility::RandomFloat();

			glm::vec3 center_(a + 0.9f * utility::RandomFloat(), 0.2f, b + 0.9f * utility::RandomFloat());

			if((center_ - glm::vec3(4.f, 0.f, 0.f)).length() > 0.9f)
			{
				if (choose_mat < 0.8f)//diffuse
				{
					list[i++] = new Sphere(center_, 0.2f,
						new Lambertian(glm::vec3(
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat())));
				}
				else if(choose_mat < 0.95f)//metal
				{
					list[i++] = new Sphere(center_, 0.2f,
						new Metal(
							glm::vec3(0.5f * (1.0f + utility::RandomFloat()),
							0.5f * (1.0f + utility::RandomFloat()),
							0.5f * (1.0f + utility::RandomFloat())),
							0.5f * utility::RandomFloat())
							);
				}
				else//glass
				{
					list[i++] = new Sphere(center_, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new Sphere(glm::vec3(0.f, 1.f, 0.f), 1.0f, new Dielectric(1.5f));
	list[i++] = new Sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, new Lambertian(glm::vec3(0.91f, 0.13f, 0.15f)));
	list[i++] = new Sphere(glm::vec3(4.f, 1.f, 0.f), 1.f, new Metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));
	return new HittableList(list, i);
}

int main()
{
	Renderer* renderer = new Renderer();
	
	Hittable* list[] = {
		new Sphere(glm::vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(glm::vec3(0.1f, 0.2f, 0.5f))),
		new Sphere(glm::vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f))),
		new Sphere(glm::vec3(1.f, 0.f, -1.f), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		new Sphere(glm::vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f)),
	};
	renderer->SetWorld(new HittableList(list, 4));
	
	auto start_running = std::chrono::system_clock::now();
	
	std::thread rendering_thread([&renderer]
	{
		while (!renderer->WindowShouldClose())
		{

			renderer->RenderFrames();
		}
	});
	
	while (!renderer->WindowShouldClose())
	{
		renderer->Tick();
	}
	
	rendering_thread.join();

	std::chrono::duration<double> runtime = std::chrono::system_clock::now() - start_running;
	std::cout << renderer->GetSamples() <<" spp in " << runtime.count() << " seconds = "
		<< renderer->GetSamples() / runtime.count() << " sps " << (runtime.count() / renderer->GetSamples()) * 1000.f << " ms.";
	delete(renderer);
	return 0;
}
