#include "GUI.hpp"

GUI::GUI() noexcept(false){
	if(!initializeGLFW()){

	}
}






















const bool GUI::initializeGLFW() noexcept(false){
	if(glfwInit() != GLFW_TRUE){
		return(false);
	}

	



	return(true);
}