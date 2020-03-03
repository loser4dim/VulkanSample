#pragma once

#define WIN32_LEAN_AND_MEAN




#define GLFW_INCLUDE_VULKAN

//#include <vulkan/vulkan.hpp>
#include <vulkan/vk_layer.h>

#pragma comment(lib, "vulkan-1")

#include <cmath>

class alignas(sizeof(float_t)) VulkanRenderer final : Renderer{
public:
	VulkanRenderer() noexcept(false);
	VulkanRenderer(const VulkanRenderer&) = delete;
	VulkanRenderer(VulkanRenderer&&) = delete;
	VulkanRenderer& operator=(const VulkanRenderer&) = delete;
	VulkanRenderer& operator=(VulkanRenderer&&) = delete;
	~VulkanRenderer() noexcept(false);

private:
	VkInstance instance_;
	VkDevice logic_device_;
	VkPhysicalDevice physical_device_;

	VkSurfaceKHR surface_;
	VkSurfaceFormatKHR surface_format_;
	VkSurfaceCapabilitiesKHR surface_capability_;

	VkPhysicalDeviceMemoryProperties physicl_device_memory_prop_;

	VkQueryPool query_pool_;

	uint32_t graphics_queue_index_ = 0;

	VkPresentModeKHR present_mode_ = VK_PRESENT_MODE_FIFO_KHR;

	uint32_t image_index_ = 0;


private:
	const bool initializeVulkan(const char* const application_name, const char* const engine_name) noexcept(false);






	const VkResult initializeInstance(const char* const application_name, const char* const engine_name) noexcept(false);



#ifdef _DEBUG
	const bool enableDebugReport() const noexcept(false);
#endif

};