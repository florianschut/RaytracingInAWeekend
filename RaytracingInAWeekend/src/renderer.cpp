#include "renderer.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <future>

#include "glad/glad.h"
#include "glfw3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"
#include "../ThirdParty/ImGui/imgui.h"
#include "../ThirdParty/ImGui/imgui_impl_opengl3.h"
#include "../ThirdParty/ImGui/imgui_impl_glfw.h"

#include "ray.hpp"
#include "material.hpp"
#include "random.hpp"

Renderer::Renderer()
{
	img_data_ = new uint8_t[nx_ * ny_ * 3];
	float_img_data_ = new float[nx_ * ny_ * 3];
	InitOpenGL();
	InitImGui();
}

Renderer::~Renderer()
{
	glfwTerminate();
	stbi_write_bmp("output.bmp", nx_, ny_, 3, img_data_);
	delete world_;
}

glm::vec3 Renderer::Color(const Ray& r, Hittable* world, unsigned int depth)
{
	HitRecord record;

	if (world->Hit(r, 0.001f, FLT_MAX, record))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < max_depth_ && record.mat_ptr->Scatter(r, record, attenuation, scattered))
		{
			return attenuation * Color(scattered, world, depth + 1);
		}
		else
		{
			return glm::vec3(0.f);
		}
	}
	else
	{
		const glm::vec3 unit_direction = normalize(r.direction);
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * glm::vec3(1.f, 1.f, 1.f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}

bool Renderer::WindowShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void Renderer::RenderFrames()
{
	unsigned int y = 0;
	const unsigned int num_cores = std::thread::hardware_concurrency();
	std::future<void>* futures = new std::future<void>[num_cores];
	samples_++;

	//TODO: This probably will cause a load of issues on a single threaded system...
	for (uint8_t i = 0; i < num_cores; i++)
	{
		futures[i] = std::async([=] {RenderSingleLine(y, samples_, img_data_, world_, camera_); });
		y++;
	}
	while (y < ny_)
	{
		for (uint8_t i = 0; i < num_cores; i++)
		{
			if (futures[i].wait_for(std::chrono::milliseconds(0)) == std::future_status::ready && y < ny_)
			{
				futures[i] = std::async([=] {RenderSingleLine(y, samples_, img_data_, world_, camera_); });
				y++;
			}
		}
	}
	for (uint8_t i = 0; i < num_cores; i++)
	{
		futures[i].get();
	}


	if (camera_.did_change_)
	{
		int array_size = nx_ * ny_ * 3;
		for (int i = 0; i < array_size; i++)
			float_img_data_[i] = 0.f;
		samples_ = 0;
		camera_.did_change_ = false;
	}
	did_render_ = true;

	}

void Renderer::SetWorld(Hittable* world)
{
	world_ = world;
}

void Renderer::Tick()
{
	glfwPollEvents();

	TickImGui();

	if (did_render_)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nx_, ny_, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data_);
		glGenerateMipmap(GL_TEXTURE_2D);
		did_render_ = false;
	}
	glUseProgram(shader_program_);
	glBindVertexArray(vao_);
	glBindTexture(GL_TEXTURE_2D, render_texture_);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window_);
}

bool Renderer::InitOpenGL()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(nx_, ny_, "Ray Tracing in One Weekend", nullptr, nullptr);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW Window...\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";

		return false;
	}
	glViewport(0, 0, nx_, ny_);

	glGenBuffers(1, &vbo_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader_source_, nullptr);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return false;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_source_, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return false;
	}

	shader_program_ = glCreateProgram();
	glAttachShader(shader_program_, vertexShader);
	glAttachShader(shader_program_, fragmentShader);
	glLinkProgram(shader_program_);

	glGetProgramiv(shader_program_, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shader_program_, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return false;
	}

	glUseProgram(shader_program_);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenTextures(1, &render_texture_);
	glBindTexture(GL_TEXTURE_2D, render_texture_);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	return true;
}

void Renderer::RenderSingleFrame(ThreadData data)
{
	try
	{
		unsigned int n = 0;
		for (int y = data.ny - 1; y >= 0; y--)
		{
			for (int x = 0; x < data.nx; x++)
			{
				float u = (static_cast<float>(x) + utility::RandomFloat()) / static_cast<float>(data.nx);
				float v = (static_cast<float>(y) + utility::RandomFloat()) / static_cast<float>(data.ny);
				glm::vec3 col = Color(data.cam.GetRay(u, v), data.world, 0);

				data.img_data[n++] +=  col.r;
				data.img_data[n++] +=  col.g;
				data.img_data[n++] +=  col.b;
			}
		}
	}
	catch (const std::exception & e)
	{
		std::cerr << "THREAD-EXCEPTION (thread: " << std::this_thread::get_id() << ")" << e.what() << std::endl;
	}
}

void Renderer::RenderSingleLine(unsigned int y, uint32_t samples, uint8_t* img_data, Hittable* world, Camera& camera)
{
	std::atomic_uint n = (ny_ - 1 - y) * nx_ * 3u;
	float v = (static_cast<float>(y) + utility::RandomFloat()) / static_cast<float>(ny_);
	for (int x = 0; x < nx_; x++)
	{
		float u = (static_cast<float>(x) + utility::RandomFloat()) / static_cast<float>(nx_);
		glm::vec3 col = Color(camera.GetRay(u, v), world, 0);
		img_data[n++] = static_cast<uint8_t>(std::round(((static_cast<float>(img_data[n] * (samples - 1)) + 255.99 * sqrt(col.r)) / static_cast<float>(samples))));
		img_data[n++] = static_cast<uint8_t>(std::round(((static_cast<float>(img_data[n] * (samples - 1)) + 255.99 * sqrt(col.g)) / static_cast<float>(samples))));
		img_data[n++] = static_cast<uint8_t>(std::round(((static_cast<float>(img_data[n] * (samples - 1)) + 255.99 * sqrt(col.b)) / static_cast<float>(samples))));
	}
}

void Renderer::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(window_), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Renderer::TickImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (camera_menu_open_)
	{
		ImGui::Begin("Camera Settings", &camera_menu_open_);

		glm::vec3 camPos = camera_.GetOrigin();

		if (ImGui::DragFloat3("Position", &camPos.x))
			camera_.SetOrigin(camPos);

		ImGui::End();
	}
	ImGui::Render();
}