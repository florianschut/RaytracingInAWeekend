#include "scenes.hpp"

#include "world.hpp"
#include "box.hpp"
#include "camera.hpp"
#include "bvh_node.hpp"
#include "lambertian.hpp"
#include "rectangle.hpp"
#include "diffuse_light.hpp"
#include "sphere.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "camera.hpp"
#include "instance.hpp"

std::shared_ptr<HittableList> Scene::EmptyCornell(std::shared_ptr<Camera>* camera, std::shared_ptr<Hittable>* lights, float aspect, glm::vec3 dimensions, float light_intensity, float light_size, bool inverse_z)
{
	auto scene = std::make_shared<HittableList>();
	auto red = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.65f, 0.05f, 0.05f)));
	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));
	auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.12f, 0.45f, 0.15f)));
	auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(light_intensity)));

	//Lights
	if (lights != nullptr)
	{
		auto square_light = std::make_shared<FlipNormals>(std::make_shared<XZRect>(-0.5f * light_size, 0.5f * light_size, -0.5f * light_size, 0.5f * light_size, 0.5f * dimensions.y - 0.001f, light));
		reinterpret_cast<std::shared_ptr<HittableList>&>(*lights)->Add(square_light);
		scene->Add(square_light);
	}
	
	//Walls
	scene->Add(std::make_shared<FlipNormals>(std::make_shared <YZRect>(-0.5f * dimensions.y, 0.5f * dimensions.y, -0.5f * dimensions.z, 0.5f * dimensions.z, 0.5f * dimensions.x, green))); //left wall
	scene->Add(std::make_shared<YZRect>(-0.5f * dimensions.y, 0.5f * dimensions.y, -0.5f * dimensions.z, 0.5f * dimensions.z, -0.5f * dimensions.z, red));                                     //right wall
	scene->Add(std::make_shared<FlipNormals>(std::make_shared<XZRect>(-0.5f * dimensions.x, 0.5f * dimensions.x, -0.5f * dimensions.z, 0.5f * dimensions.z, 0.5f * dimensions.y, white)));  //top wall
	scene->Add(std::make_shared<XZRect>(-0.5f * dimensions.x, 0.5f * dimensions.x, -0.5f * dimensions.z, 0.5f * dimensions.z, -0.5f * dimensions.y, white));                                   //bottom wall
	if (!inverse_z)
		scene->Add(std::make_shared<FlipNormals>(std::make_shared <XYRect>(-0.5f * dimensions.x, 0.5f * dimensions.x, -0.5f * dimensions.y, 0.5f * dimensions.y, 0.5f * dimensions.z, white))); //back wall
	else
		scene->Add(std::make_shared<FlipNormals>(std::make_shared <XYRect>(-0.5f * dimensions.x, 0.5f * dimensions.x, -0.5f * dimensions.y, 0.5f * dimensions.y, -0.5f * dimensions.z, white))); //back wall

	if(camera!= nullptr)
		*camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, -dimensions.z * 2.f * ((static_cast<float>(inverse_z) + 1.f))*0.5), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);
	
	return scene;
}

std::shared_ptr<World> Scene::CornellScene(float aspect)
{
	std::shared_ptr<Hittable> lights = std::make_shared<HittableList>();
	auto scene = EmptyCornell(nullptr, &lights, aspect, glm::vec3(555.f), 13.f, 130.f);

	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));
	auto aluminum = std::make_shared<Metal>(std::make_shared<ConstantTexture>(glm::vec3(0.8f, 0.85f, 0.88f)), 0.0f);
	auto glass = std::make_shared<Dielectric>(1.5f);

	auto sphere = std::make_shared<Sphere>(glm::vec3(190.f, 90.f, 190.f), 90.f, glass);
	reinterpret_cast<std::shared_ptr<HittableList>&>(lights)->Add(sphere);
	scene->Add(sphere);

	auto box1 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(165.f, 330.f, 165.f), aluminum);
	scene->Add(std::make_shared<Translate>(std::make_shared<RotateY>(box1, 15.f), glm::vec3(265.f, 0.f, 295.f)));// , 0.01f, new ConstantTexture(glm::vec3(0.f)));

	auto camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, -400.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);

	return std::make_shared<World>(std::make_shared<BvhNode>(*scene, 0.f, 0.f), lights, camera);
}

std::shared_ptr<World> Scene::CornellBunny(float aspect)
{
	std::shared_ptr<Hittable> lights = std::make_shared<HittableList>();
	auto scene = EmptyCornell(nullptr, &lights, aspect, glm::vec3(2.5f), 0.f, 1.f, true);

	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));

	auto mesh = std::make_shared<Mesh>("assets/models/bunny.obj", white);
	scene->Add(std::make_shared<Translate>(mesh, glm::vec3(0.25f, -1.25f, -0.5f)));
	scene->Add(std::make_shared<Sphere>(glm::vec3(-1.25f), 0.5f, std::make_shared<Metal>(std::make_shared<ConstantTexture>(glm::vec3(1.f)), 0.f)));

	auto camera = std::make_shared<Camera>(glm::vec3(0, 0, 4.5f), glm::vec3(0, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);

	auto background = std::make_shared<CubeMapBackground>("assets/textures/cave_skybox/");
	return std::make_shared<World>(std::make_shared<BvhNode>(*scene, 0.f, 1.f), lights, camera, background);
}

std::shared_ptr<World> Scene::CornellDragon(float aspect)
{
	std::shared_ptr<Hittable> lights = std::make_shared<HittableList>();

	auto scene = EmptyCornell(nullptr, &lights, aspect, glm::vec3(2.5f), 13.f, 1.f);

	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));

	auto mesh = std::make_shared<Mesh>("assets/models/dragon.obj", white);
	scene->Add(std::make_shared<Translate>(mesh, glm::vec3(0.f, -1.f, 0.f)));
	auto camera = std::make_shared<Camera>(glm::vec3(0, 0, -2.0f), glm::vec3(0, -1.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);
	return std::make_shared<World>(std::make_shared<BvhNode>(*scene, 0.f, 1.f), lights, camera);
}


std::shared_ptr<World> Scene::CornellTeapot(float aspect)
{
	std::shared_ptr<Hittable> lights = std::make_shared<HittableList>();
	auto scene = EmptyCornell(nullptr, &lights, aspect, glm::vec3(255.f), 13.f, 90.f);
	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));
	
	auto teapot_mesh = std::make_shared<Mesh>("assets/models/teapot.obj", white);
	scene->Add(std::make_shared<Translate>(teapot_mesh, glm::vec3(127.5f, 0.f, 127.5f)));

	auto camera = std::make_shared<Camera>(glm::vec3(0, 0, -80.f), glm::vec3(0.f, -72.f, 50.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);

	return std::make_shared<World>(std::make_shared<BvhNode>(*scene, 0.f, 1.f), lights, camera);
}


std::shared_ptr<World> Scene::TriangleBoxScene(float aspect)
{
	std::shared_ptr<Hittable> lights = std::make_shared<HittableList>();
	auto scene = EmptyCornell(nullptr, &lights, aspect, glm::vec3(5.55f), 3.f, 3.3f);
	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));

	float vertices[] = {
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};


	auto box_mesh = std::make_shared<Mesh>(vertices, 6u, 0u, 3u, 12u, white);
	//auto triangle = std::make_shared<Triangle>(glm::vec3(vertices[0], vertices[1], vertices[2]), glm::vec3(vertices[5], vertices[6], vertices[7]), glm::vec3(vertices[10], vertices[11], vertices[12]), white);
	auto box1 = std::make_shared<Box>(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f), white);
	scene->Add(std::make_shared<Translate>(/*std::make_shared<RotateY>(*/box_mesh/*, 15.f)*/, glm::vec3(2.85f, 0.5f, 2.95f)));

	auto camera = std::make_shared<Camera>(glm::vec3(0, 0, -4.00f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f), 40.0f, aspect, 0.0f, 10.0f, 0.f, 1.f);

	return std::make_shared<World>(std::make_shared<BvhNode>(*scene, 0.f, 1.f), lights, camera);
}




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
