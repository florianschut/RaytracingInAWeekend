#pragma once
#include <atomic>
#include <chrono>
#include <memory>

#include <glm/glm.hpp>

class Ray;
struct GLFWwindow;
class UserInterface;
class World;

class Renderer
{
public:
	Renderer();
	~Renderer();

	static glm::vec3 Color(const Ray& r, std::shared_ptr<World> world, unsigned int depth);

	bool WindowShouldClose() const;

	void RenderFrames();

	void SetWorld(std::shared_ptr<World> world);

	void Tick();
	
	GLFWwindow* GetCurrentWindow() const { return window_; }

	std::shared_ptr<World> GetWorld() const { return world_; };
	
	uint32_t GetSamples() const { return samples_; }

	void SaveOutputToFile(const char* name = "output.bmp");
	
	std::chrono::time_point<std::chrono::system_clock> start_running_;
	std::chrono::time_point<std::chrono::system_clock> previous_render_;
	std::chrono::time_point<std::chrono::system_clock> last_render_;

	private:
	bool InitOpenGL();
	static inline void RenderSingleLine(unsigned int y, float* img_data, std::shared_ptr<World> world);

	std::atomic<bool> did_render_ = false;

	static const uint16_t nx_ = 1280;
	static const uint16_t ny_ = 720;
	float* img_data_;

	std::atomic<uint32_t> samples_ = 0;
	static const uint16_t max_depth_ = 50;

	std::shared_ptr<World> world_ = nullptr;

	std::shared_ptr<UserInterface> user_interface_;
	
	GLFWwindow* window_ = nullptr;
	unsigned int vbo_ = 0;
	unsigned int vao_ = 0;
	unsigned int render_texture_ = 0;
	unsigned int shader_program_ = 0;
	unsigned int samples_uniform_ = 0;

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
