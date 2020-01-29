#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "../ThirdParty/ImGui/imgui.h"
#include "../ThirdParty/ImGui/imgui_impl_opengl3.h"
#include "../ThirdParty/ImGui/imgui_impl_glfw.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_img/stb_image_write.h"

#include "renderer.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "random.hpp"
#include "material.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "metal.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"

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

	ImGui::Begin("Camera Settings", &camera_menu_open);

	glm::vec3 camPos = camera.get_origin();
	
	if (ImGui::DragFloat3("Position", &camPos.x))
	{
		cam_did_change = true;
		camera.set_origin(camPos);
	}
	
	ImGui::End();

	ImGui::Render();
}



hittable* random_scene()
{
	const int n = 500;
	auto list = new hittable * [n + 1];
	list[0] = new sphere(glm::vec3(0.f, -1000.f, 0.f), 1000.f, new lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			const float choose_mat = random_float();

			glm::vec3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if((center - glm::vec3(4.f, 0.f, 0.f)).length() > 0.9f)
			{
				if (choose_mat < 0.8f)//diffuse
				{
					list[i++] = new sphere(center, 0.2f,
						new lambertian(glm::vec3(
							random_float() * random_float(),
							random_float() * random_float(),
							random_float() * random_float())));
				}
				else if(choose_mat < 0.95f)//metal
				{
					list[i++] = new sphere(center, 0.2f,
						new metal(
							glm::vec3(0.5f * (1.0f + random_float()),
							0.5f * (1.0f + random_float()),
							0.5f * (1.0f + random_float())),
							0.5f * random_float())
							);
				}
				else//glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new sphere(glm::vec3(0.f, 1.f, 0.f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, new lambertian(glm::vec3(0.91f, 0.13f, 0.15f)));
	list[i++] = new sphere(glm::vec3(4.f, 1.f, 0.f), 1.f, new metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f));
	return new hittable_list(list, i);
}

int main()
{
	Renderer* renderer = new Renderer();
	initImGui(renderer->GetCurrentWindow());
	
	const auto img_data = new uint8_t[nx * ny * 3];
	const auto float_img_data = new float[nx * ny * 3];
	
	camera cam(glm::vec3(5.f, 1.5f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),75.f, static_cast<float>(nx) / static_cast<float>(ny));

	hittable* list[] = {
		new sphere(glm::vec3(0.f, 0.f, -1.f), 0.5f, new lambertian(glm::vec3(0.1f, 0.2f, 0.5f))),
		new sphere(glm::vec3(0.f, -100.5f, -1.f), 100.f, new lambertian(glm::vec3(0.8f, 0.8f, 0.0f))),
		new sphere(glm::vec3(1.f, 0.f, -1.f), 0.5f, new metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		new sphere(glm::vec3(-1.f, 0.f, -1.f), 0.5f, new dielectric(1.5f)),
		//new sphere(glm::vec3(-1.f, 0.0f, -1.f), -0.475f, new dielectric(1.5f)),
	};
	hittable* world = new hittable_list(list, 4);
	
	unsigned int samples = 0;
	bool did_render = false;
	auto start_running = std::chrono::system_clock::now();
	
	std::thread rendering_thread([&window, &ny, &nx, &spp, &world, &cam, &float_img_data, &img_data, &samples, &did_render]
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
				img_data[i] = uint8_t(255.99f * sqrt(float_img_data[i] / static_cast<float>(samples)));

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
