#pragma once
#include <atomic>

#include <glm/glm.hpp>

#include "hittable.hpp"
#include "camera.hpp"

struct GLFWwindow;

struct ThreadData {
	ThreadData(uint16_t ny, uint16_t nx, uint16_t spp, Hittable* world, Camera& cam, float* img_data) :
		ny(ny), nx(nx), spp(spp), world(world), cam(cam), img_data(img_data) {};
	uint16_t ny;
	uint16_t nx;
	uint16_t spp;
	Hittable* world;
	Camera& cam;
	float* img_data;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	static glm::vec3 Color(const Ray& r, Hittable* world, unsigned int depth);

	bool WindowShouldClose() const;

	void RenderFrames();

	void SetWorld(Hittable* world);

	void Tick();

	GLFWwindow* GetCurrentWindow() const
	{
		return window_;
	}

	Camera camera_ = Camera(glm::vec3(5.f, 1.5f, 3.f), glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.f, 1.f, 0.f), 90.f, static_cast<float>(nx_) / static_cast<float>(ny_));

	uint32_t GetSamples() const
	{
		return samples_;
	}

private:
	bool InitOpenGL();
	static void RenderSingleFrame(ThreadData data);
	static void RenderSingleLine(unsigned int y, uint32_t samples, uint8_t* img_data, Hittable* world, Camera& camera);
	void InitImGui();
	void TickImGui();

	std::atomic<bool> did_render_ = false;

	static const uint16_t nx_ = 1280;
	static const uint16_t ny_ = 720;
	uint8_t* img_data_;
	float* float_img_data_;

	std::atomic<uint32_t> samples_ = 0;
	static const uint16_t spp_ = 256;
	static const uint8_t thread_count_ = 11;
	static const uint16_t max_depth_ = 50;

	Hittable* world_ = nullptr;

	GLFWwindow* window_ = nullptr;
	unsigned int vbo_ = 0;
	unsigned int vao_ = 0;
	unsigned int vertex_shader_ = 0;
	unsigned int render_texture_ = 0;
	unsigned int shader_program_ = 0;

	bool camera_menu_open_ = true;

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
