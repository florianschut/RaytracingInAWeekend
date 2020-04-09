#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "renderer.hpp"
#include "scenes.hpp"


int main()
{
	Renderer* renderer = new Renderer();

	std::shared_ptr<Hittable> lights = nullptr;
	
	renderer->SetWorld(scene::CornelScene(renderer->camera_, lights, 16.f/9.f));
	renderer->SetLights(lights);
	
	const auto start_running = std::chrono::system_clock::now();
	
	std::thread rendering_thread([&renderer]
	{
		while (!renderer->WindowShouldClose())
		{

			renderer->RenderFrames();
		}
	});
	
	while (!renderer->WindowShouldClose())
	{
		renderer->Tick();
	}
	
	rendering_thread.join();

	std::chrono::duration<double> runtime = std::chrono::system_clock::now() - start_running;
	std::cout << renderer->GetSamples() <<" spp in " << runtime.count() << " seconds = "
		<< renderer->GetSamples() / runtime.count() << " sps " << (runtime.count() / renderer->GetSamples()) * 1000.f << " ms.";
	delete(renderer);
	return 0;
}
