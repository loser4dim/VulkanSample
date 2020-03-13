#pragma once

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

//#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
//#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>

//#include <vulkan/vulkan.hpp>
#include <vulkan/vk_layer.h>
//#include <vulkan/vulkan_win32.h>

#pragma comment(lib, "glfw3")
#pragma comment(lib, "vulkan-1")
#pragma comment(lib, "VkLayer_utils")
#pragma comment(lib, "shaderc_combined")

#include <cmath>

class alignas(sizeof(float_t)) Renderer{
public:
	Renderer() noexcept = default;
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;
	~Renderer() noexcept = default;

	virtual void render() noexcept(false) = 0;
};