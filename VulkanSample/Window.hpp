#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3")

#include <cmath>

class alignas(sizeof(float_t)) Window final{
public:
	Window() = delete;
	Window(GLFWwindow* const new_window) noexcept(false);
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
	~Window() noexcept = default;

private:
	GLFWwindow* window_ = nullptr;




public:




};