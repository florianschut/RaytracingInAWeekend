#include <chrono>
#include <iostream>
#include <future>

#include "glad/glad.h"
#include "glfw3.h"

#include "../ThirdParty/ImGui/imgui.h"
#include "../ThirdParty/ImGui/imgui_impl_opengl3.h"
#include "../ThirdParty/ImGui/imgui_impl_glfw.h"

#include "ray.h"
#include "vec3.h"
#include "camera.h"
#include "random.h"
#include "material.h"

#include "hittable_list.h"
#include "sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
float vertices[] =
{
	-1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 
	-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f
};

const char* vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec2 TexCoord;\n"
"void main() {\n"
"	TexCoord = vec2((aPos.x + 1.f) / 2.f, (aPos.y + 1.f) / -2.f);\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);\n }\0";

const char* fragmentShaderSource =
"#version 460 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D ourTexture;\n"
"void main() {\n"
"FragColor = texture(ourTexture, TexCoord);}\n";

const unsigned int max_depth = 50;

void initImGui(GLFWwindow* window)
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
	ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(window), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

bool camera_menu_open = true;
bool cam_did_change = false;
void tickImGui(camera& camera)
{
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("Camera", &camera_menu_open);

	vec3 camPos = camera.get_origin();

	if (ImGui::DragFloat3("Position", &camPos.x))
	{
		cam_did_change = true;
		camera.set_origin(camPos);
	}
	
	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();

	ImGui::Render();
	
}

vec3 color(const ray& r, hittable* world, unsigned int depth)
{
	hit_record record;

	if (world->hit(r, 0.001f, FLT_MAX, record))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < max_depth && record.mat_ptr->scatter(r, record, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.f);
		}
	}
	else
	{
		const vec3 unit_direction = unit_vector(r.direction);
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}
struct thread_data {
	thread_data(int ny, int nx, int spp, hittable* world, camera& cam, float *img_data) :
		ny(ny), nx(nx), spp(spp), world(world), cam(cam), img_data(img_data) {};
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
	for (int y = data.ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < data.nx; x++)
		{
			float u = float(x + random_double()) / float(data.nx);
			float v = float(y + random_double()) / float(data.ny);
			vec3 col = color(data.cam.get_ray(u, v), data.world, 0);
			
			data.img_data[n++] += col.r;
			data.img_data[n++] += col.g;
			data.img_data[n++] += col.b;
		}
	}
}

hittable* random_scene()
{
	int n = 500;
	hittable** list = new hittable * [n + 1];
	list[0] = new sphere(vec3(0.f, -1000.f, 0.f), 1000.f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = random_double();

			vec3 center(a + 0.9f * float(random_double()), 0.2f, b + 0.9f * float(random_double()));

			if((center - vec3(4.f, 0.f, 0.f)).length() > 0.9f)
			{
				if (choose_mat < 0.8)//diffuse
				{
					list[i++] = new sphere(center, 0.2f,
						new lambertian(vec3(
							float(random_double() * random_double()),
							float(random_double() * random_double()),
							float(random_double() * random_double()))));
				}
				else if(choose_mat < 0.95f)//metal
				{
					list[i++] = new sphere(center, 0.2f,
						new metal(
							vec3(0.5f * float(1.0 + random_double()),
							0.5f * float(1.0 + random_double()),
							0.5f * float(1.0 + random_double())),
							float(0.5 * random_double()))
							);
				}
				else//glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0.f, 1.f, 0.f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.f, 1.f, 0.f), 1.f, new lambertian(vec3(0.91f, 0.13f, 0.15f)));
	list[i++] = new sphere(vec3(4.f, 1.f, 0.f), 1.f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0));
	return new hittable_list(list, i);
}

int main()
{
	const unsigned int nx = 1280;
	const unsigned int ny = 720;
	const unsigned int spp = 256;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(nx, ny, "Ray Tracing in One Weekend", nullptr, nullptr);
	if(window == nullptr)
	{
		std::cout << "Failed to create GLFW Window...\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		
		return -1;
	}
	glViewport(0, 0, nx, ny);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
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
		return -1;
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	initImGui(window);
	
	const auto img_data = new uint8_t[nx * ny * 3];
	const auto float_img_data = new float[nx * ny * 3];
	
	camera cam(vec3(5.f, 1.5f, 3.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f),75.f, float(nx) / float(ny));

	hittable* list[] = {
		new sphere(vec3(0.f, 0.f, -1.f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f))),
		new sphere(vec3(0.f, -100.5f, -1.f), 100.f, new lambertian(vec3(0.8f, 0.8f, 0.0f))),
		new sphere(vec3(1.f, 0.f, -1.f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		new sphere(vec3(-1.f, 0.f, -1.f), 0.5f, new dielectric(1.5f)),
		//new sphere(vec3(-1.f, 0.0f, -1.f), -0.475f, new dielectric(1.5f)),
	};
	hittable* world = new hittable_list(list, 4);
	
	unsigned int n = 0;
	unsigned int samples = 0;
	bool did_render = false;
	auto start_running = std::chrono::system_clock::now();

	std::thread rendering_thread([&window, &ny, &nx, &spp, &world, &cam, &float_img_data, &img_data, &samples, &shaderProgram, &texture, &VAO, &did_render]
	{
		const unsigned int thread_count = 6;
		while (!glfwWindowShouldClose(window))
		{
			auto start_render = std::chrono::system_clock::now();

			thread_data data_for_threads(ny, nx, spp, world, cam, float_img_data);
			std::thread* threads[thread_count] = {};
			for (int i = 0; i < thread_count; i++)
			{
				threads[i] = new std::thread(render_frames, data_for_threads);
			}
			for (int i = 0; i < thread_count; i++)
			{
				threads[i]->join();
			}

			samples += thread_count;
			
			int array_size = nx * ny * 3;
			for (int i = 0; i < array_size; i++)
				img_data[i] = uint8_t(255.99 * sqrt(float_img_data[i] / float(samples)));

			if (cam_did_change)
			{
				for (int i = 0; i < array_size; i++)
					float_img_data[i] = 0.f;
				samples = 0;
				cam_did_change = false;
			}
			

			did_render = true;

			std::chrono::duration<double> render_duration = std::chrono::system_clock::now() - start_render;
			std::cout << "Rendering took " << render_duration.count() << " seconds. Sample count is "<< samples<<std::endl;
		}
	});
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		tickImGui(cam);
		
		if (did_render)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nx, ny, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
			glGenerateMipmap(GL_TEXTURE_2D);
			did_render = false;
		}
	
		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		glfwSwapBuffers(window);
	}
	
	rendering_thread.join();

	std::chrono::duration<double> runtime = std::chrono::system_clock::now() - start_running;
	std::cout << samples<<" were rendered in " << runtime.count() << " seconds.\n";

	glfwTerminate();
	stbi_write_bmp("output.bmp", nx, ny, 3, img_data);
	return 0;
}
