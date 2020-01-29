#include "renderer.hpp"

#include "glad/glad.h"
#include "glfw3.h"

Renderer::Renderer()
{
	InitOpenGL();
}

glm::vec3 Renderer::color(const ray& r, hittable* world, unsigned int depth)
{
	hit_record record;

	if (world->hit(r, 0.001f, FLT_MAX, record))
	{
		ray scattered;
		glm::vec3 attenuation;
		if (depth < max_depth_ && record.mat_ptr->scatter(r, record, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
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

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return false;
	}

	glUseProgram(shaderProgram);
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

void Renderer::RenderFrames(ThreadData data)
{
	try
	{
		unsigned int n = 0;
		for (int y = data.ny - 1; y >= 0; y--)
		{
			for (int x = 0; x < data.nx; x++)
			{
				float u = (static_cast<float>(x) + random_float()) / static_cast<float>(data.nx);
				float v = (static_cast<float>(y) + random_float()) / static_cast<float>(data.ny);
				glm::vec3 col = color(data.cam.get_ray(u, v), data.world, 0);

				data.img_data[n++] += col.r;
				data.img_data[n++] += col.g;
				data.img_data[n++] += col.b;
			}
		}
	}
	catch (const std::exception & e)
	{
		std::cerr << "THREAD-EXCEPTION (thread: " << std::this_thread::get_id() << ")" << e.what() << std::endl;
	}
}
