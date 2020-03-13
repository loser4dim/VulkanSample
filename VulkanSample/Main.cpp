
#include "GUI.hpp"
#include "OpenGLRenderer.hpp"
#include "VulkanRenderer.hpp"

#include <iostream>

using namespace std;

const int main(const size_t argc, const char* const* const argv){
#ifdef _DEBUG
	for(size_t i(0); i < argc; ++i){
		clog << argv[i] << endl;
	}
#endif

	constexpr const char* APPLICATION_TITLE_ = "Vulkan Sample";

	GUI gui;

	auto window = glfwCreateWindow(960, 540, APPLICATION_TITLE_, nullptr, nullptr);


	

	/*Renderer* renderer = nullptr;
	if(gui.isSupportedVulkan()){
		renderer = new VulkanRenderer(APPLICATION_TITLE_, APPLICATION_TITLE_, window);
	}
	else{
		renderer = new OpenGLRenderer();
	}*/

	while(glfwWindowShouldClose(window) == GLFW_FALSE){
		glfwPollEvents();
		//renderer->render();
	}


	//delete renderer;
	glfwTerminate();

	return EXIT_SUCCESS;
}