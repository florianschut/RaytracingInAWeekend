#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ray.h"
#include "vec3.h"
#include "hittable_list.h"
#include "sphere.h"

vec3 color(const ray& r, hittable* world)
{
	hit_record record;

	if (world->hit(r, 0.0, FLT_MAX, record))
	{
		return 0.5f * vec3(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1);
	}
	else
	{
		const vec3 unit_direction = unit_vector(r.direction);
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	const unsigned int nx = 1920;
	const unsigned int ny = 1080;

	const auto img_data = new uint8_t[nx * ny * 3];

	const float aspect_ratio = float(nx) / float(ny);
	vec3 lower_left_corner(-aspect_ratio, -1.f, -1.f);
	vec3 horizontal(2* aspect_ratio, 0.f, 0.f);
	vec3 vertical(0.f, 2.f, 0.f);
	vec3 origin(0.f, 0.f, 0.f);

	hittable* list[2];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f);
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100.f);
	hittable* world = new hittable_list(list, 2);
	
	unsigned int n = 0;
	for(int y = ny-1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			float u = float(x) / float(nx);
			float v = float(y) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 col = color(r, world);

			img_data[n++] = int(255.99 * col.r);
			img_data[n++] = int(255.99 * col.g);
			img_data[n++] = int(255.99 * col.b);
		}
	}
	
	stbi_write_bmp("output.bmp", nx, ny, 3, img_data);
}
