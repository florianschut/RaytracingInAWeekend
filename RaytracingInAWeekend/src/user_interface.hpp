#pragma once

class Renderer;
struct GLFWwindow;

class UserInterface
{
public:
	UserInterface(GLFWwindow* window);
	~UserInterface();

	//Update UserInterface after rendering texture but before swap buffer
	void Tick(Renderer& renderer);

private:
	//Contains position settings of the camera
	void CameraMenu(Renderer& renderer);
	bool camera_menu_open_ = true;
};