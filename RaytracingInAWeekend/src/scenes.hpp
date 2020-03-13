#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_img/stb_image.h"

#include "random.hpp"
#include "sphere.hpp"
#include "rectangle.hpp"
#include "moving_sphere.hpp"
#include "metal.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"
#include "bvh_node.hpp"
#include "diffuse_light.hpp"
#include "box.hpp"
#include "instance.hpp"
#include "volumes.hpp"

namespace scene
{
	Hittable* RandomScene()
	{
		const int n = 500;
		auto list = new Hittable * [n + 1];
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

				if ((center_ - glm::vec3(4.f, 0.f, 0.f)).length() > 0.9f)
				{
					if (choose_mat < 0.4f)//Moving diffuse
					{
						list[i++] = new MovingSphere(center_,
							center_ + glm::vec3(0.0f, 0.5f * utility::RandomFloat(), 0.f),
							0.f, 1.f, 0.2f,
							new Lambertian(new ConstantTexture(glm::vec3(
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
					else if (choose_mat < 0.95f)//metal
					{
						list[i++] = new Sphere(center_, 0.2f,
							new Metal(new ConstantTexture(glm::vec3(
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
		list[i++] = new Sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, new Lambertian(new ConstantTexture(glm::vec3(0.91f, 0.13f, 0.15f))));
		list[i++] = new Sphere(glm::vec3(4.f, 1.f, 0.f), 1.f, new Metal(new ConstantTexture(glm::vec3(0.7f, 0.6f, 0.5f)), 0.0f));
		return new BvhNode(list, i, 0.0f, 1.f);
	}

	Hittable* SimpleScene()
	{
		const int n = 4;
		auto list = new Hittable * [n];

		int nx, ny, nn;
		unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

		list[0] = new Sphere(glm::vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(new ImageTexture(tex_data, nx, ny)));
		list[1] = new Sphere(glm::vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(new ConstantTexture(glm::vec3(0.8f, 0.8f, 0.0f))));
		list[2] = new Sphere(glm::vec3(1.f, 0.f, -1.f), 0.5f, new Metal(new ConstantTexture(glm::vec3(0.8f, 0.6f, 0.2f)), 0.3f));
		list[3] = new Sphere(glm::vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f));

		return new BvhNode(list, n, 0.f, 1.f);
	}

	Hittable* PerlinScene()
	{
		auto* perlin_texture = new NoiseTexture(12);
		const int n = 4;
		auto list = new Hittable * [n];
		list[0] = new Sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new Lambertian(perlin_texture));
		list[1] = new Sphere(glm::vec3(0.f, 2.f, 0.f), 2.f, new Lambertian(perlin_texture));
		list[2] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(glm::vec3(4))));
		list[3] = new Sphere(glm::vec3(0.f, 7.f, 0.f), 2.f, new DiffuseLight(new ConstantTexture(glm::vec3(4))));
		return new BvhNode(list, n, 0.f, 0.f);
	}

	Hittable* CornelScene()
	{
		auto list = new Hittable * [8];
		int i = 0;
		Material* red = new Lambertian(new ConstantTexture(glm::vec3(0.65, 0.05, 0.05)));
		Material* white = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));;
		Material* green = new Lambertian(new ConstantTexture(glm::vec3(0.12, 0.45, 0.15)));;
		Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(7)));

		list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
		list[i++] = new YZRect(0, 555, 0, 555, 0, red);
		list[i++] = new XZRect(113, 443, 127, 432, 554, light);
		list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
		list[i++] = new XZRect(0, 555, 0, 555, 0, white);
		list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
		list[i++] = new ConstantMedium( new Translate(new RotateY( new Box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), -18.f), glm::vec3(130, 0, 65)), 0.01f, new ConstantTexture(glm::vec3(1.f)));
		list[i++] = new ConstantMedium( new Translate(new RotateY( new Box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 15.f), glm::vec3(265, 0, 295)), 0.01f, new ConstantTexture(glm::vec3(0.f)));

		return new BvhNode(list, i, 0.f, 0.f);
	}
}
