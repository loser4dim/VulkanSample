#pragma once

#include "Renderer.hpp"
#include <vector>

class alignas(sizeof(float_t)) VulkanRenderer final : public Renderer{
public:
	VulkanRenderer() = delete;
	VulkanRenderer(const char* const application_name, const char* const engine_name, GLFWwindow* const window) noexcept(false);
	VulkanRenderer(const VulkanRenderer&)				= delete;
	VulkanRenderer(VulkanRenderer&&)					= delete;
	VulkanRenderer& operator=(const VulkanRenderer&)	= delete;
	VulkanRenderer& operator=(VulkanRenderer&&)			= delete;
	~VulkanRenderer() noexcept(false);

private:
	static VkBool32 VKAPI_CALL DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, const uint64_t objct, const size_t location, const int32_t message_code, const char* const layer_prefix, const char* const message, void* const user_data);

private:
	VkInstance instance_;
	VkPhysicalDevice first_physical_device_;
	VkPhysicalDeviceMemoryProperties physical_device_memory_prop_;
	uint32_t graphics_queue_index_ = 0;

#ifdef _DEBUG
	PFN_vkCreateDebugReportCallbackEXT debug_report_create_callback_;
	PFN_vkDebugReportMessageEXT debug_report_message_;
	PFN_vkDestroyDebugReportCallbackEXT debug_report_destroy_callback_;
	VkDebugReportCallbackEXT debug_report_;
#endif

	VkDevice logic_device_;
	VkQueue device_queue_;

	VkCommandPool command_pool_;
	VkPresentModeKHR present_mode_ = VK_PRESENT_MODE_FIFO_KHR;

	VkSurfaceKHR surface_;
	VkSurfaceFormatKHR surface_format_;
	VkSurfaceCapabilitiesKHR surface_capability_;

	VkSwapchainKHR swapchain_;
	VkExtent2D swapchain_extent_;
	std::vector<VkImage> swapchain_images_;
	std::vector<VkImageView> swapchain_image_views_;

	VkImage depth_buffer_;
	VkDeviceMemory depth_buffer_memory_;
	VkImageView depth_buffer_view;

	VkRenderPass render_pass_;

	std::vector<VkFramebuffer> framebuffers_;

	std::vector<VkFence> fences_;
	VkSemaphore render_completed_semaphore_;
	VkSemaphore present_completed_semaphore_;

	std::vector<VkCommandBuffer> command_buffers_;


	uint32_t image_index_ = 0;


	//VkQueryPool query_pool_;





public:
	const uint32_t calcMemoryTypeIndex(uint32_t request_bits, const VkMemoryPropertyFlags request_prop_flags) const noexcept;
	
	void prepare() noexcept(false);
	void clean() noexcept(false);
	void render() noexcept(false) override;
	void makeCommand(const VkCommandBuffer command) noexcept(false);

private:
	const VkResult initialize(const char* const application_name, const char* const engine_name, GLFWwindow* const window) noexcept(false);
	const VkResult terminate() noexcept(false);

	const VkResult initializeInstance(const char* const application_name, const char* const engine_name) noexcept(false);
	const VkResult selectPhysicalDevice() noexcept(false);
	const uint32_t searchGraphicsQueueIndex() noexcept(false);
#ifdef _DEBUG
	void enableDebugReport() noexcept(false);
	void disableDebugReport() noexcept(false);
#endif
	const VkResult createLogicDevice() noexcept(false);
	const VkResult creatCommandPool() noexcept(false);
	const VkResult selectSurfaceFormat(const VkFormat format) noexcept(false);
	const VkResult createSwapchain() noexcept(false);
	const VkResult createDepthBuffer() noexcept(false);
	const VkResult createImageView() noexcept(false);
	const VkResult createRenderPass() noexcept(false);
	const VkResult createFramebuffer() noexcept(false);
	const VkResult createCommandBuffer() noexcept(false);
	const VkResult createSemaphore() noexcept(false);
};