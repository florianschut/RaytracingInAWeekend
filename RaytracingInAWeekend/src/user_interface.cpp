#include "user_interface.hpp"



#include <chrono>
#include <glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glm/vec3.hpp>

#include "renderer.hpp"



UserInterface::UserInterface(GLFWwindow* window)
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

UserInterface::~UserInterface()
{
}

void UserInterface::Tick(Renderer& renderer)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	InfoMenu(renderer);
	
	ImGui::Render();

	const auto io = ImGui::GetIO();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UserInterface::InfoMenu(Renderer& renderer)
{
	ImGui::Begin("Info!");
	std::chrono::duration<float> runtime = std::chrono::system_clock::now() - renderer.start_running_;
	std::chrono::duration<float> last_pass = renderer.last_render_ - renderer.previous_render_;

	ImGui::Text("%i Samples per pixel rendered over %.1f seconds. \nLast pass took %.4f ms.", renderer.GetSamples(), runtime.count(), last_pass.count());
	glm::vec3 camPos = renderer.camera_.GetOrigin();

	if (ImGui::DragFloat3("Position", &camPos.x))
		renderer.camera_.SetOrigin(camPos);
	ImGui::Separator();
	static char name_buf[128] = "output.bmp";
	ImGui::InputText("File name", name_buf, 128);
	ImGui::SameLine();
	if (ImGui::Button("Save to file"))
		renderer.SaveOutputToFile(name_buf);
	ImGui::End();

}
