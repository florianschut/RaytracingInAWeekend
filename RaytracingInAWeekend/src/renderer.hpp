#pragma once
#include <atomic>

#include <glm/glm.hpp>

#include "hittable.hpp"
#include "camera.hpp"

struct GLFWwindow;

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

	Camera camera_ = Camera(glm::vec3(800, 278, -525.f), glm::vec3(278, 278, 0.f), glm::vec3(0.f, 1.f, 0.f), 40.f,
		static_cast<float>(nx_) / static_cast<float>(ny_), 0.0f, glm::length(glm::vec3(13.f, 2.f, 3.f) - glm::vec3(0.f, 1.0f, 0.f)), 0.f, 1.f);

	uint32_t GetSamples() const
	{
		return samples_;
	}

private:
	bool InitOpenGL();
	static inline void RenderSingleLine(unsigned int y, float* img_data, Hittable* world, Camera& camera);
	void InitImGui();
	void TickImGui();

	std::atomic<bool> did_render_ = false;

	static const uint16_t nx_ = 1280;
	static const uint16_t ny_ = 720;
	float* img_data_;

	std::atomic<uint32_t> samples_ = 0;
	static const uint16_t max_depth_ = 50;

	Hittable* world_ = nullptr;

	GLFWwindow* window_ = nullptr;
	unsigned int vbo_ = 0;
	unsigned int vao_ = 0;
	unsigned int render_texture_ = 0;
	unsigned int shader_program_ = 0;
	unsigned int samples_uniform_ = 0;

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
		"uniform float samples;\n"
		"uniform sampler2D ourTexture;\n"
		"void main() {\n"
		"FragColor = sqrt(texture(ourTexture, TexCoord) / samples);}\n";
};
