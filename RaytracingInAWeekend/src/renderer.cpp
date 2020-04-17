#include "renderer.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <future>

#include "glad/glad.h"
#include "glfw3.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

#include "ray.hpp"
#include "material.hpp"
#include "random.hpp"
#include "pdf.hpp"
#include "textures.hpp"
#include "user_interface.hpp"
#include "world.hpp"

Renderer::Renderer()
{
	img_data_ = new float[nx_ * ny_ * 3];
	InitOpenGL();
	user_interface_ = std::make_shared<UserInterface>(window_);
}

Renderer::~Renderer()
{
	glfwTerminate();
	SaveOutputToFile();
	delete[] img_data_;
#ifdef  _DEBUG
	if(!world_.unique())
		std::cerr << "Renderer::world_ is still being referenced!\n";
#endif
}

glm::vec3 Renderer::Color(const Ray& r, std::shared_ptr<World> world, unsigned int depth)
{
	if (depth >= max_depth_)
		return glm::vec3(0);
	
	HitRecord rec;
	if (!world->Hit(r, 0.001f, FLT_MAX, rec))
		return world->BackgroundColor(r);

	ScatterRecord srec;
	glm::vec3 emitted = rec.mat_ptr->Emitted(r, rec);
	if(!rec.mat_ptr->Scatter(r, rec, srec))
		return emitted;
	
	if(srec.specular)
		return srec.attenuation * Color(srec.specular_ray, world, depth + 1);

	MixturePdf pdf(world->LightPdf(rec), srec.pdf);

	Ray scattered = Ray(rec.p, pdf.Generate(), r.Time());
	auto pdf_val = pdf.Value(scattered.Direction());
	
	return emitted + srec.attenuation * rec.mat_ptr->ScatteringPdf(r, rec, scattered) * Color(scattered, world,depth + 1) / pdf_val;
}

bool Renderer::WindowShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void Renderer::RenderFrames()
{
	if (did_render_ == true) //Don't intervene with texture copy, could and should probably be handled more efficiently
		return;
	
	if (camera_.did_change_)
	{
		int array_size = nx_ * ny_ * 3;
		for (int i = 0; i < array_size; i++)
			img_data_[i] = 0.f;
		samples_ = 0;
		camera_.did_change_ = false;
		start_running_ = std::chrono::system_clock::now();
	}

	unsigned int y = 0;
	const auto num_cores = std::thread::hardware_concurrency();
	const auto futures = new std::future<void>[num_cores];

	for (uint8_t i = 0; i < num_cores; i++)
	{
		futures[i] = std::async([=] {RenderSingleLine(y, img_data_, world_); });
		++y;
	}
	while (y < ny_)
	{
		for (uint8_t i = 0; i < num_cores; i++)
		{
			if (futures[i].wait_for(std::chrono::milliseconds(0)) == std::future_status::ready && y < ny_)
			{
				futures[i] = std::async([=] {RenderSingleLine(y, img_data_, world_); });
				++y;
			}
		}
	}
	for (uint8_t i = 0; i < num_cores; i++)
	{
		futures[i].get();
	}

	++samples_;
	did_render_ = true;
	delete[] futures;
}

void Renderer::SetWorld(std::shared_ptr<World> world)
{
	world_ = world;
}

void Renderer::Tick()
{
	if (did_render_)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, nx_, ny_, 0, GL_RGB, GL_FLOAT, img_data_);
		did_render_ = false;
		previous_render_ = last_render_;
		last_render_ = std::chrono::system_clock::now();
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1f(samples_uniform_, static_cast<float>(samples_)); 
	}
	glfwPollEvents();

	glUseProgram(shader_program_);
	glBindVertexArray(vao_);
	glBindTexture(GL_TEXTURE, render_texture_);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	user_interface_->Tick(*this);
	
	glfwSwapBuffers(window_);
}

void Renderer::SaveOutputToFile(const char* name)
{
	const auto array_size = nx_ * ny_ * 3;
	const auto output_img = new uint8_t[array_size];
	for (auto i = 0; i < array_size; i++)
	{
		output_img[i] = static_cast<uint8_t>(255.99f * sqrt(std::min(img_data_[i] / static_cast<float>(samples_), 1.f)));
	}
	stbi_write_bmp(name, nx_, ny_, 3, output_img);
	delete[] output_img;
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

	const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
	samples_uniform_ = glGetUniformLocation(shader_program_, "samples");

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

void Renderer::RenderSingleLine(unsigned int y, float* img_data, std::shared_ptr<World> world)
{
	std::atomic_uint n = (ny_ - 1 - y) * nx_ * 3u;
	const auto v = (static_cast<float>(y) + utility::RandomFloat()) / static_cast<float>(ny_);
	for (int x = 0; x < nx_; x++)
	{
		const auto u = (static_cast<float>(x) + utility::RandomFloat()) / static_cast<float>(nx_);
		auto col = Color(world->GetCamera()->GetRay(u, v), world, 0);
		if (col.r != col.r) col.r = 0.0f;
		if (col.g != col.g) col.g = 0.0f;
		if (col.b != col.b) col.b = 0.0f;
		img_data[n++] += col.r;
		img_data[n++] += col.g;
		img_data[n++] += col.b;
	}
}


