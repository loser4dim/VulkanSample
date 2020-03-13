#include "GUI.hpp"

using namespace std;

GUI::GUI() noexcept(false){
	initializeGLFW();

}



GUI::~GUI() noexcept(false){

}


















const bool GUI::initializeGLFW() noexcept(false){
	if(glfwInit() != GLFW_TRUE){
		return(false);
	}

	is_supported_vulkan_ = (glfwVulkanSupported() == GLFW_TRUE);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	return(true);
}


