


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include <string>

using namespace std;

const int main(const size_t argc, const char* const* const argv){
#ifdef _DEBUG
	for(size_t i(0); i < argc; ++i){
		clog << argv[i] << endl;
	}
#endif















	return EXIT_SUCCESS;
}