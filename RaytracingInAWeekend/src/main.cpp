#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"

#include "renderer.hpp"
#include "random.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "moving_sphere.hpp"
#include "metal.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"
#include "bvh_node.hpp"

Hittable* RandomScene()
{
	const int n = 500;
	auto list = new Hittable* [n + 1];
	Texture* checker_tex = new CheckerTexture(
		new ConstantTexture(glm::vec3(0.2f, 0.3f, 0.1f)),
		new ConstantTexture(glm::vec3(0.9f, 0.9f, 0.9f)));
	
	list[0] = new Sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new Lambertian(checker_tex));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			const float choose_mat = utility::RandomFloat();

			glm::vec3 center_(a + 0.9f * utility::RandomFloat(), 0.2f, b + 0.9f * utility::RandomFloat());

			if((center_ - glm::vec3(4.f, 0.f, 0.f)).length() > 0.9f)
			{
				if (choose_mat < 0.4f)//Moving diffuse
				{
					list[i++] = new MovingSphere(center_, 
						center_+ glm::vec3(0.0f, 0.5f * utility::RandomFloat(), 0.f), 
						0.f, 1.f, 0.2f,
						new Lambertian(new ConstantTexture( glm::vec3(
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat()))));
				}
				else if (choose_mat < 0.8f)//diffuse
				{
					list[i++] = new Sphere(center_, 0.2f,
						new Lambertian(new ConstantTexture(glm::vec3(
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat(),
							utility::RandomFloat() * utility::RandomFloat()))));
				}
				else if(choose_mat < 0.95f)//metal
				{
					list[i++] = new Sphere(center_, 0.2f,
						new Metal(new ConstantTexture( glm::vec3(
							0.5f * (1.0f + utility::RandomFloat()),
							0.5f * (1.0f + utility::RandomFloat()),
							0.5f * (1.0f + utility::RandomFloat()))),
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
	list[i++] = new Sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, new Lambertian( new ConstantTexture( glm::vec3(0.91f, 0.13f, 0.15f))));
	list[i++] = new Sphere(glm::vec3(4.f, 1.f, 0.f), 1.f, new Metal(new ConstantTexture(glm::vec3(0.7f, 0.6f, 0.5f)), 0.0f));
	return new BvhNode(list, i, 0.0f, 1.f);
}

Hittable* SimpleScene()
{
	const int n = 4;
	auto list = new Hittable* [n];

	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	
	list[0] = new Sphere(glm::vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(new ImageTexture(tex_data, nx, ny)));
	list[1] = new Sphere(glm::vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(new ConstantTexture(glm::vec3(0.8f, 0.8f, 0.0f))));
	list[2] = new Sphere(glm::vec3(1.f, 0.f, -1.f), 0.5f, new Metal(new ConstantTexture(glm::vec3(0.8f, 0.6f, 0.2f)), 0.3f));
	list[3] = new Sphere(glm::vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f));

	return new BvhNode( list, n, 0.f, 1.f);
}

Hittable* PerlinScene()
{
	auto* perlin_texture = new NoiseTexture(12);
	const int n = 2;
	auto list = new Hittable * [n];
	list[0] = new Sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new Lambertian(perlin_texture));
	list[1] = new Sphere(glm::vec3(0.f, 2.f, 0.f), 2.f, new Lambertian(perlin_texture));
	return new BvhNode(list, n, 0.f, 0.f);
}

int main()
{
	Renderer* renderer = new Renderer();
	
	renderer->SetWorld(PerlinScene());
	
	const auto start_running = std::chrono::system_clock::now();
	
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
