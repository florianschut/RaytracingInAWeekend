#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "renderer.hpp"
#include "scenes.hpp"

int main()
{
	Renderer* renderer = new Renderer();

	std::shared_ptr<Hittable> lights = nullptr;
	
	renderer->SetWorld(Scene::CornellBunny(16.f/9.f));
	
	renderer->start_running_ = std::chrono::system_clock::now();
	
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

	std::chrono::duration<double> runtime = std::chrono::system_clock::now() - renderer->start_running_;
	std::cout << renderer->GetSamples() <<" spp in " << runtime.count() << " seconds = "
		<< renderer->GetSamples() / runtime.count() << " sps " << (runtime.count() / renderer->GetSamples()) * 1000.f << " ms.";
	delete(renderer);
	return 0;
}
