#include <chrono>
#include <iostream>
#include <future>

#include "ray.h"
#include "vec3.h"
#include "camera.h"
#include "random.h"

#include "hittable_list.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

vec3 random_in_unit_sphere()
{
	vec3 retval;
	do 
	{
		retval = 2 * vec3(random_double(), random_double(), random_double()) - vec3(1.f);
	} while (retval.squared_length() >= 1.0f);
	return retval;
}

vec3 color(const ray& r, hittable* world)
{
	hit_record record;

	if (world->hit(r, 0.001f, FLT_MAX, record))
	{
		vec3 target = record.p + record.normal + random_in_unit_sphere();
		return 0.5f * color(ray(record.p, target - record.p), world);
	}
	else
	{
		const vec3 unit_direction = unit_vector(r.direction);
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}
struct thread_data {
	thread_data(unsigned int thread_count, int ny, int nx, int spp, hittable* world, camera& cam, float *img_data) :
		thread_count(thread_count), ny(ny), nx(nx), spp(spp), world(world), cam(cam), img_data(img_data) {};
	unsigned int thread_count;
	int ny;
	int nx;
	int spp;
	hittable* world;
	camera& cam;
	float* img_data;
};

void render_frames(thread_data data)
{
	unsigned int n = 0;
	const unsigned int thread_spp = data.spp / data.thread_count;
	for (int y = data.ny - 1; y >= 0; y--)
	{
		/*data_for_threads.y = y;
		data_for_threads.img_data = &img_data[(ny - (y + 1)) * nx * 3];
		std::thread t1(render_horizontal_line, data_for_threads);

		t1.join();*/

		for (int x = 0; x < data.nx; x++)
		{
			vec3 col(0);
			for (int s = 0; s < thread_spp; s++)
			{
				float u = float(x + random_double()) / float(data.nx);
				float v = float(y + random_double()) / float(data.ny);
				col += color(data.cam.get_ray(u, v), data.world);
			}
			//col /= float(thread_spp);
			//col = vec3(sqrt(col.r), sqrt(col.g), sqrt(col.b));
			
			data.img_data[n++] += col.r;
			data.img_data[n++] += col.g;
			data.img_data[n++] += col.b;
		}
	}
}
int main()
{
	const unsigned int nx = 1980;
	const unsigned int ny = 1080;
	const unsigned int spp = 25;

	const auto img_data = new uint8_t[nx * ny * 3];
	const auto float_img_data = new float[nx * ny * 3];

	const float aspect_ratio = float(nx) / float(ny);
	camera cam(aspect_ratio);

	hittable* list[2];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f);
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100.f);
	hittable* world = new hittable_list(list, 2);
	
	auto start_render = std::chrono::system_clock::now();
	unsigned int n = 0;

	/*auto render_horizontal_line = [](thread_data dat) {
		for (int x = 0; x < dat.nx; x++)
		{
			vec3 col(0);
			for (int s = 0; s < dat.spp; s++)
			{
				float u = float(x + random_double()) / float(dat.nx);
				float v = float(y + random_double()) / float(dat.ny);
				col += color(dat.cam.get_ray(u, v), dat.world);
			}
			col /= float(dat.spp);
			col = vec3(sqrt(col.r), sqrt(col.g), sqrt(col.b));

			dat.img_data[x] = int(255.99 * col.r);
			dat.img_data[x + 1] = int(255.99 * col.g);
			dat.img_data[x + 2] = int(255.99 * col.b);
		}
	};*/

	

	/*for(int y = ny-1; y >= 0; y--)
	{*/
		thread_data data_for_threads(4, ny, nx, spp, world, cam, float_img_data);
		std::thread t1(render_frames, data_for_threads);
		std::thread t2(render_frames, data_for_threads);
		std::thread t3(render_frames, data_for_threads);
		std::thread t4(render_frames, data_for_threads);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		int array_size = nx * ny * 3;
		for (int i = 0; i < array_size; i++)
			img_data[i] = uint8_t(255.99 * sqrt(float_img_data[i] / spp));

		/*for (int x = 0; x < nx; x++)
		{
			vec3 col(0);
			for (int s = 0; s < spp; s++)
			{
				float u = float(x + random_double()) / float(nx);
				float v = float(y + random_double()) / float(ny);
				col += color(cam.get_ray(u, v), world);
			}
			col /= float(spp);
			col = vec3(sqrt(col.r), sqrt(col.g), sqrt(col.b));
			
			img_data[n++] = int(255.99 * col.r);
			img_data[n++] = int(255.99 * col.g);
			img_data[n++] = int(255.99 * col.b);
		}*/
	//}

	std::chrono::duration<double> render_duration = std::chrono::system_clock::now() - start_render;
	std::cout << "Rendering took " << render_duration.count() << " seconds.\n" ;
	stbi_write_bmp("output.bmp", nx, ny, 3, img_data);
}
