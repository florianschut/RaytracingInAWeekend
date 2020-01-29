#pragma once
#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "random.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"

struct ThreadData {
	ThreadData(int ny, int nx, int spp, hittable* world, camera& cam, float* img_data) :
		ny(ny), nx(nx), spp(spp), world(world), cam(cam), img_data(img_data) {};
	int ny;
	int nx;
	int spp;
	hittable* world;
	camera& cam;
	float* img_data;
};

class Renderer
{
public:
	Renderer();

	glm::vec3 color(const ray& r, hittable* world, unsigned int depth);

	GLFWwindow* GetCurrentWindow() const
	{
		return window_;
	}

private:
	bool InitOpenGL();
	void RenderFrames(ThreadData data);

	const uint16_t nx_ = 1280;
	const uint16_t ny_ = 720;

	const uint16_t max_depth_ = 50;

	GLFWwindow* window_ = nullptr;
	unsigned int vbo_ = 0;
	unsigned int vao_ = 0;
	unsigned int vertex_shader_ = 0;
	unsigned int render_texture_ = 0;

	const float vertices_[12] =
	{
		-1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f
	};

	const char* vertex_shader_source_ =
		"#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec2 TexCoord;\n"
		"void main() {\n"
		"	TexCoord = vec2((aPos.x + 1.f) / 2.f, (aPos.y + 1.f) / -2.f);\n"
		"   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);\n }\0";

	const char* fragment_shader_source_ =
		"#version 460 core\n"
		"in vec2 TexCoord;\n"
		"out vec4 FragColor;\n"
		"uniform sampler2D ourTexture;\n"
		"void main() {\n"
		"FragColor = texture(ourTexture, TexCoord);}\n";

	
};