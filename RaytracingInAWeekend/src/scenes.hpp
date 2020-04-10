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
#include "hittable_list.hpp"

namespace scene
{
	/* needs to be switched to std::shared_ptr*/
	//Hittable* RandomScene()
	//{
	//	const int n = 500;
	//	auto list = new Hittable * [n + 1];
	//	Texture* checker_tex = new CheckerTexture(
	//		new ConstantTexture(glm::vec3(0.2f, 0.3f, 0.1f)),
	//		new ConstantTexture(glm::vec3(0.9f, 0.9f, 0.9f)));

	//	list[0] = new Sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new Lambertian(checker_tex));
	//	int i = 1;
	//	for (int a = -11; a < 11; a++)
	//	{
	//		for (int b = -11; b < 11; b++)
	//		{
	//			const float choose_mat = utility::RandomFloat();

	//			glm::vec3 center_(a + 0.9f * utility::RandomFloat(), 0.2f, b + 0.9f * utility::RandomFloat());

	//			if ((center_ - glm::vec3(4.f, 0.f, 0.f)).length() > 0.9f)
	//			{
	//				if (choose_mat < 0.4f)//Moving diffuse
	//				{
	//					list[i++] = new MovingSphere(center_,
	//						center_ + glm::vec3(0.0f, 0.5f * utility::RandomFloat(), 0.f),
	//						0.f, 1.f, 0.2f,
	//						new Lambertian(new ConstantTexture(glm::vec3(
	//							utility::RandomFloat() * utility::RandomFloat(),
	//							utility::RandomFloat() * utility::RandomFloat(),
	//							utility::RandomFloat() * utility::RandomFloat()))));
	//				}
	//				else if (choose_mat < 0.8f)//diffuse
	//				{
	//					list[i++] = new Sphere(center_, 0.2f,
	//						new Lambertian(new ConstantTexture(glm::vec3(
	//							utility::RandomFloat() * utility::RandomFloat(),
	//							utility::RandomFloat() * utility::RandomFloat(),
	//							utility::RandomFloat() * utility::RandomFloat()))));
	//				}
	//				else if (choose_mat < 0.95f)//metal
	//				{
	//					list[i++] = new Sphere(center_, 0.2f,
	//						new Metal(new ConstantTexture(glm::vec3(
	//							0.5f * (1.0f + utility::RandomFloat()),
	//							0.5f * (1.0f + utility::RandomFloat()),
	//							0.5f * (1.0f + utility::RandomFloat()))),
	//							0.5f * utility::RandomFloat())
	//					);
	//				}
	//				else//glass
	//				{
	//					list[i++] = new Sphere(center_, 0.2f, new Dielectric(1.5f));
	//				}
	//			}
	//		}
	//	}
	//	list[i++] = new Sphere(glm::vec3(0.f, 1.f, 0.f), 1.0f, new Dielectric(1.5f));
	//	list[i++] = new Sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, new Lambertian(new ConstantTexture(glm::vec3(0.91f, 0.13f, 0.15f))));
	//	list[i++] = new Sphere(glm::vec3(4.f, 1.f, 0.f), 1.f, new Metal(new ConstantTexture(glm::vec3(0.7f, 0.6f, 0.5f)), 0.0f));
	//	return new BvhNode(list, i, 0.0f, 1.f);
	//}

	/* needs to be switched to std::shared_ptr*/
	/*Hittable* SimpleScene()
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
	}*/

	std::shared_ptr<Hittable> CornelScene(Camera& camera, std::shared_ptr<Hittable>& lights, float aspect)
	{
		std::shared_ptr<HittableList> scene = std::make_shared<HittableList>();
		Material* red = new Lambertian(new ConstantTexture(glm::vec3(0.65, 0.05, 0.05)));
		Material* white = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));
		Material* green = new Lambertian(new ConstantTexture(glm::vec3(0.12, 0.45, 0.15)));
		Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(15)));
		
		scene->Add(std::make_shared<FlipNormals>(new YZRect(0, 555, 0, 555, 555, green)));
		scene->Add(std::make_shared<YZRect>(0.f, 555.f, 0.f, 555.f, 0.f, red));
		//scene.Add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
		lights = std::make_shared<FlipNormals>(new XZRect(213, 343, 227, 332, 554, light));

		scene->Add(lights);
		
		scene->Add(std::make_shared<FlipNormals>(new XZRect(0.f, 555.f, 0.f, 555.f, 555.f, white)));
		scene->Add(std::make_shared<XZRect>(0.f, 555.f, 0.f, 555.f, 0.f, white));
		scene->Add(std::make_shared<FlipNormals>(new XYRect(0, 555, 0, 555, 555, white)));
		scene->Add(std::make_shared<Translate>(new RotateY(new Box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), -18.f), glm::vec3(130, 0, 65)));// , 0.01f, new ConstantTexture(glm::vec3(1.f))));
		scene->Add(std::make_shared<Translate>(new RotateY(new Box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 15.f), glm::vec3(265, 0, 295)));// , 0.01f, new ConstantTexture(glm::vec3(0.f)));
		camera = Camera(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), glm::vec3(0, 1, 0), 40.0, aspect, 0.0, 10.0, 0, 1);
		return scene;
//		return std::make_shared<BvhNode>(scene, 0.f, 0.f);
	}

	/* needs to be switched to std::shared_ptr*/
	/*Hittable* TheNextWeek()
	{
		int nb = 20;
		Hittable** list = new Hittable * [30];
		Hittable** box_list = new Hittable * [400];
		Hittable** box_list2 = new Hittable * [1000];

		Material* white = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));
		Material* ground = new Lambertian(new ConstantTexture(glm::vec3(0.48, 0.83, 0.53)));
		Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(7)));

		int b = 0;
		for (int i = 0; i < nb; i++)
		{
			for (int j = 0; j < nb; j++)
			{
				float w = 100.f;
				float x0 = -1000.f + static_cast<float>(i) * w;
				float z0 = -1000.f + static_cast<float>(j) * w;
				float y0 = 0;
				float x1 = x0 + w;
				float y1 = 100.f * (utility::RandomFloat() + 0.01f);
				float z1 = z0 + w;
				box_list[b++] = new Box(glm::vec3(x0, y0, z0), glm::vec3(x1, y1, z1), ground);
			}
		}

		glm::vec3 center(400, 400, 200);
		int l = 0;
		list[l++] = new BvhNode(box_list, b, 0, 1);
		list[l++] = new XZRect(123, 423, 147, 412, 554, light);
		list[l++] = new MovingSphere(center, center + glm::vec3(30, 0, 0), 0, 1, 50.f, new Lambertian(new ConstantTexture(glm::vec3(0.7f, 0.3f, 0.1f))));
		list[l++] = new Sphere(glm::vec3(260, 150, 45), 50, new Dielectric(1.5f));
		list[l++] = new Sphere(glm::vec3(0, 150, 140), 50.0f, new Metal(new ConstantTexture(glm::vec3(0.8f, 0.8f, 0.9f)), 10.0f));
		Hittable* boundry = new Sphere(glm::vec3(360, 150, 145), 70, new Dielectric(1.5));
		list[l++] = boundry;
		list[l++] = new ConstantMedium(boundry, 0.2f, new ConstantTexture(glm::vec3(0.2f, 0.4f, 0.9f)));
		boundry = new Sphere(glm::vec3(0,0,0), 5000, new Dielectric(1.5));
		list[l++] = new ConstantMedium(boundry, 0.0001f, new ConstantTexture(glm::vec3(0.2f, 0.4f, 0.9f)));
		int nx, ny, nn;
		unsigned char* texture_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
		Material* emat = new Lambertian(new ImageTexture(texture_data, nx, ny));
		list[l++] = new Sphere(glm::vec3(400, 200, 400), 100, emat);
		int ns = 1000;
		for (int j = 0; j < ns; j++)
			box_list2[j] = new Sphere(glm::vec3(165 * utility::RandomFloat(), 165 * utility::RandomFloat(), 165 * utility::RandomFloat()), 10, white);
		list[l++] = new Translate(new RotateY(new BvhNode(box_list2, ns, 0.0, 1.0), 15), glm::vec3(-100, 270, 395));
		return new BvhNode(list, l, 0.0, 1.0);	
	}*/
	
}
