#include "VulkanRenderer.hpp"
#include <array>
#include <algorithm>
#include <iostream>

using namespace std;

VulkanRenderer::VulkanRenderer(const char* const application_name, const char* const engine_name, GLFWwindow* const window) noexcept(false){
	initialize(application_name, engine_name, window);
}
VulkanRenderer::~VulkanRenderer() noexcept(false){
	terminate();
}


#ifdef _DEBUG
VkBool32 VKAPI_CALL VulkanRenderer::DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, const uint64_t objct, const size_t location, const int32_t message_code, const char* const layer_prefix, const char* const message, void* const user_data){
	VkBool32 ret = VK_FALSE;
	if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT || flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT){
		ret = VK_TRUE;
	}
	if(layer_prefix != nullptr){
		cerr << "[" << layer_prefix << "]";
	}
	cerr << message << endl;
	return ret;
}
#endif

const uint32_t VulkanRenderer::calcMemoryTypeIndex(uint32_t request_bits, const VkMemoryPropertyFlags request_prop_flags) const noexcept{
	uint32_t ret =(numeric_limits<uint32_t>::max)();
	bool loop_flag = false;
	for(uint32_t i = 0, end = physical_device_memory_prop_.memoryTypeCount; i < end && !loop_flag; ++i){
		if(request_bits & 1){
			const auto& types = physical_device_memory_prop_.memoryTypes[i];
			if((types.propertyFlags & request_prop_flags) == request_prop_flags){
				ret = i;
				loop_flag = true;
			}
		}
		request_bits >>= 1;
	}
	return ret;
}

void VulkanRenderer::prepare() noexcept(false){
	return;
}

void VulkanRenderer::clean() noexcept(false){
	return;
}

void VulkanRenderer::render() noexcept(false){
	uint32_t next_image_index = 0;
	vkAcquireNextImageKHR(logic_device_, swapchain_, (numeric_limits<uint64_t>::max)(), present_completed_semaphore_, VK_NULL_HANDLE, &next_image_index);
	auto command_fence = fences_[static_cast<size_t>(next_image_index)];
	vkWaitForFences(logic_device_, 1, &command_fence, VK_TRUE, (numeric_limits<uint64_t>::max)());

	array<VkClearValue, 2> clear_value = {
		{
			{0.5f, 0.25f, 0.25f, 0.0f},
			{1.0f, 0}
		}
	};

	VkRenderPassBeginInfo render_pass_begin_info;
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.pNext = nullptr;
	render_pass_begin_info.renderPass = render_pass_;
	render_pass_begin_info.framebuffer = framebuffers_[next_image_index];
	render_pass_begin_info.renderArea.offset = VkOffset2D{0, 0};
	render_pass_begin_info.renderArea.extent = swapchain_extent_;
	render_pass_begin_info.clearValueCount = static_cast<uint32_t>(clear_value.size());
	render_pass_begin_info.pClearValues = clear_value.data();

	VkCommandBufferBeginInfo command_buffer_begin_info;
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.pNext = nullptr;
	command_buffer_begin_info.flags = 0;
	command_buffer_begin_info.pInheritanceInfo = nullptr;

	auto& command = command_buffers_[next_image_index];
	vkBeginCommandBuffer(command, &command_buffer_begin_info);
	vkCmdBeginRenderPass(command, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	image_index_ = next_image_index;
	makeCommand(command);

	vkCmdEndRenderPass(command);
	vkEndCommandBuffer(command);

	VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info;
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = nullptr;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &present_completed_semaphore_;
	submit_info.pWaitDstStageMask = &wait_stage_mask;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &render_completed_semaphore_;

	vkResetFences(logic_device_, 1, &command_fence);
	vkQueueSubmit(device_queue_, 1, &submit_info, command_fence);

	VkPresentInfoKHR present_info;
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext = nullptr;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &render_completed_semaphore_;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &swapchain_;
	present_info.pImageIndices = &next_image_index;
	present_info.pResults = nullptr;

	vkQueuePresentKHR(device_queue_, &present_info);

	return;
}

void VulkanRenderer::makeCommand(const VkCommandBuffer command) noexcept(false){
	return;
}




const VkResult VulkanRenderer::initialize(const char* const application_name, const char* const engine_name, GLFWwindow* const window) noexcept(false){
	initializeInstance(application_name, engine_name);

	cout << "baaaaka" << endl;
	selectPhysicalDevice();
	graphics_queue_index_ = searchGraphicsQueueIndex();

	cout << "baaaaka" << endl;
#ifdef _DEBUG
	enableDebugReport();

	cout << "baaaaka" << endl;
#endif
	createLogicDevice();

	cout << "baaaaka" << endl;
	creatCommandPool();


	cout << "baaaaka" << endl;
	glfwCreateWindowSurface(instance_, window, nullptr, &surface_);


	cout << "baaaaka" << endl;
	selectSurfaceFormat(VK_FORMAT_B8G8R8A8_UNORM);

	cout << "baaaaka" << endl;
	createSwapchain();

	cout << "baaaaka" << endl;
	createDepthBuffer();

	cout << "baaaaka" << endl;
	createImageView();

	cout << "baaaaka" << endl;
	createRenderPass();

	cout << "baaaaka" << endl;
	createFramebuffer();

	cout << "baaaaka" << endl;
	createCommandBuffer();

	cout << "baaaaka" << endl;
	createSemaphore();

	cout << "baaaaka" << endl;
	return VK_SUCCESS;
}

const VkResult VulkanRenderer::terminate() noexcept(false){
	vkDeviceWaitIdle(logic_device_);

	vkFreeCommandBuffers(logic_device_, command_pool_, static_cast<uint32_t>(command_buffers_.size()), command_buffers_.data());
	command_buffers_.clear();

	vkDestroyRenderPass(logic_device_, render_pass_, nullptr);
	for(auto& it : framebuffers_){
		vkDestroyFramebuffer(logic_device_, it, nullptr);
	}
	framebuffers_.clear();

	vkFreeMemory(logic_device_, depth_buffer_memory_, nullptr);
	vkDestroyImage(logic_device_, depth_buffer_, nullptr);
	vkDestroyImageView(logic_device_, depth_buffer_view, nullptr);

	for(auto& it : swapchain_images_){
		vkDestroyImage(logic_device_, it, nullptr);
	}
	swapchain_images_.clear();
	for(auto& it : swapchain_image_views_){
		vkDestroyImageView(logic_device_, it, nullptr);
	}
	swapchain_image_views_.clear();
	vkDestroySwapchainKHR(logic_device_, swapchain_, nullptr);

	for(auto& it : fences_){
		vkDestroyFence(logic_device_, it, nullptr);
	}
	fences_.clear();

	vkDestroySemaphore(logic_device_, present_completed_semaphore_, nullptr);
	vkDestroySemaphore(logic_device_, render_completed_semaphore_, nullptr);

	vkDestroyCommandPool(logic_device_, command_pool_, nullptr);

	vkDestroySurfaceKHR(instance_, surface_, nullptr);

	vkDestroyDevice(logic_device_, nullptr);

#ifdef _DEBUG
	disableDebugReport();
#endif

	vkDestroyInstance(instance_, nullptr);

	return VK_SUCCESS;
}















const VkResult VulkanRenderer::initializeInstance(const char* const application_name, const char* const engine_name) noexcept(false){
	//Make Info for Application
	VkApplicationInfo application_info;
	application_info.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//application_info.pNext				= nullptr;
	application_info.pApplicationName	= application_name;
	//application_info.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
	application_info.pEngineName		= application_name;
	application_info.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
	application_info.apiVersion			= VK_API_VERSION_1_1;

	//Get Extension
	uint32_t cnt = 0;
	VkResult status = VK_SUCCESS;
	status = vkEnumerateInstanceExtensionProperties(nullptr, &cnt, nullptr);
	if(status != VK_SUCCESS){
		return status;
	}
	vector<VkExtensionProperties> extension_props(static_cast<size_t>(cnt));
	status = vkEnumerateInstanceExtensionProperties(nullptr, &cnt, extension_props.data());
	if(status != VK_SUCCESS){
		return status;
	}
	vector<const char*> extension_names;
	for(const auto& it : extension_props){
		extension_names.push_back(it.extensionName);
		cout << it.extensionName << endl;
	}

	cout << extension_props.size() << endl;
	cout << extension_names.size() << endl;

	//Make Info for Creating Instance
	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	//instance_create_info.pNext = nullptr;
	//instance_create_info.flags = 0;
	instance_create_info.pApplicationInfo = &application_info;
#ifdef _DEBUG
	const char* const layer[] = {"VK_LAYER_LUNARG_standard_validation"};
	instance_create_info.enabledLayerCount = 1;
	instance_create_info.ppEnabledLayerNames = layer;
#elif
	instance_create_info.enabledLayerCount = 0;
	instance_create_info.ppEnabledLayerNames = nullptr;
#endif
	instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	instance_create_info.ppEnabledExtensionNames = extension_names.data();

	//Create Instance
	return vkCreateInstance(&instance_create_info, nullptr, &instance_);
}

const VkResult VulkanRenderer::selectPhysicalDevice() noexcept(false){
	uint32_t cnt = 0;
	VkResult status = VK_SUCCESS;
	status = vkEnumeratePhysicalDevices(instance_, &cnt, nullptr);
	if(status != VK_SUCCESS){
		return status;
	}
	vector<VkPhysicalDevice> physical_devices(static_cast<size_t>(cnt));
	first_physical_device_ = physical_devices.front();
	vkGetPhysicalDeviceMemoryProperties(first_physical_device_, &physical_device_memory_prop_);
	return VK_SUCCESS;
}

const uint32_t VulkanRenderer::searchGraphicsQueueIndex() noexcept(false){
	//Search Queue Family
	uint32_t cnt = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(first_physical_device_, &cnt, nullptr);
	vector<VkQueueFamilyProperties> queue_family_props(static_cast<size_t>(cnt));
	vkGetPhysicalDeviceQueueFamilyProperties(first_physical_device_, &cnt, queue_family_props.data());
	//Search Queue Index
	uint32_t queue_index = 0;
	bool loop_flag = false;
	for(uint32_t i = 0; i < cnt && !loop_flag; ++i){
		if(queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			queue_index = i;
			loop_flag = true;
		}
	}
	if(!loop_flag){
		queue_index = (numeric_limits<uint32_t>::max)();
	}
	return queue_index;
}

#ifdef _DEBUG
void VulkanRenderer::enableDebugReport() noexcept(false){
	debug_report_create_callback_ = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance_, "vkCreateDebugReportCallbackEXT"));
	debug_report_message_ = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(instance_, "vkDebugReportMessageEXT"));
	debug_report_destroy_callback_ = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance_, "vkDestroyDebugReportCallbackEXT"));

	VkDebugReportFlagsEXT debug_report_flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

	VkDebugReportCallbackCreateInfoEXT debug_report_callback_create_info;
	debug_report_callback_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debug_report_callback_create_info.pNext = nullptr;
	debug_report_callback_create_info.flags = debug_report_flags;
	debug_report_callback_create_info.pfnCallback = &DebugReportCallback;
	debug_report_callback_create_info.pUserData = nullptr;

	debug_report_create_callback_(instance_, &debug_report_callback_create_info, nullptr, &debug_report_);
	return;
}

void VulkanRenderer::disableDebugReport() noexcept(false){
	if(debug_report_destroy_callback_){
		debug_report_destroy_callback_(instance_, debug_report_, nullptr);
	}
	return;
}
#endif

const VkResult VulkanRenderer::createLogicDevice() noexcept(false){
	const float_t default_queue_priority = 1.0f;
	VkDeviceQueueCreateInfo device_queue_create_info;
	device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.pNext = nullptr;
	device_queue_create_info.flags = 0;
	device_queue_create_info.queueFamilyIndex = graphics_queue_index_;
	device_queue_create_info.queueCount = 1;
	device_queue_create_info.pQueuePriorities = &default_queue_priority;

	uint32_t cnt = 0;
	VkResult status = VK_SUCCESS;
	status = vkEnumerateDeviceExtensionProperties(first_physical_device_, nullptr, &cnt, nullptr);
	if(status != VK_SUCCESS){
		return status;
	}
	vector<VkExtensionProperties> extension_props(static_cast<size_t>(cnt));
	status = vkEnumerateDeviceExtensionProperties(first_physical_device_, nullptr, &cnt, extension_props.data());
	if(status != VK_SUCCESS){
		return status;
	}

	vector<const char*> extension_names;
	for(const auto& it : extension_props){
		extension_names.push_back(it.extensionName);
	}

	VkDeviceCreateInfo device_create_info;

	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext = nullptr;
	device_create_info.flags = 0;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &device_queue_create_info;
	device_create_info.enabledLayerCount = 0;
	device_create_info.ppEnabledLayerNames = nullptr;
	device_create_info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	device_create_info.ppEnabledExtensionNames = extension_names.data();
	device_create_info.pEnabledFeatures = nullptr;

	status = vkCreateDevice(first_physical_device_, &device_create_info, nullptr, &logic_device_);
	if(status != VK_SUCCESS){
		return status;
	}

	vkGetDeviceQueue(logic_device_, graphics_queue_index_, 0, &device_queue_);
	return VK_SUCCESS;
}

const VkResult VulkanRenderer::creatCommandPool() noexcept(false){
	VkCommandPoolCreateInfo command_pool_create_info;
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.pNext = nullptr;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = graphics_queue_index_;

	return vkCreateCommandPool(logic_device_, &command_pool_create_info, nullptr, &command_pool_);
}

const VkResult VulkanRenderer::selectSurfaceFormat(const VkFormat format) noexcept(false){
	uint32_t cnt = 0;
	VkResult status = VK_SUCCESS;
	status = vkGetPhysicalDeviceSurfaceFormatsKHR(first_physical_device_, surface_, &cnt, nullptr);
	if(status != VK_SUCCESS){
		return status;
	}
	vector<VkSurfaceFormatKHR> surface_formats(static_cast<size_t>(cnt));
	status = vkGetPhysicalDeviceSurfaceFormatsKHR(first_physical_device_, surface_, &cnt, surface_formats.data());
	if(status != VK_SUCCESS){
		return status;
	}

	for(const auto& it: surface_formats){
		if(it.format == format){
			surface_format_ = it;
		}
	}
	
	status = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(first_physical_device_, surface_, &surface_capability_);
	if(status != VK_SUCCESS){
		return status;
	}

	VkBool32 is_supported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(first_physical_device_, graphics_queue_index_, surface_, &is_supported);
	if(is_supported != VK_TRUE){
		return VK_INCOMPLETE;
	}

	return VK_SUCCESS;
}

const VkResult VulkanRenderer::createSwapchain() noexcept(false){
	const uint32_t image_cnt = max(2u, surface_capability_.minImageCount);
	VkExtent2D extent = surface_capability_.currentExtent;
	if(extent.width == (numeric_limits<uint32_t>::max)()){
		extent.width = 1920;
		extent.height = 1080;
	}

	uint32_t queue_family_indcides[] = {graphics_queue_index_};
	VkSwapchainCreateInfoKHR swapchain_create_info;
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.pNext = nullptr;
	swapchain_create_info.flags = 0;
	swapchain_create_info.surface = surface_;
	swapchain_create_info.minImageCount = image_cnt;
	swapchain_create_info.imageFormat = surface_format_.format;
	swapchain_create_info.imageColorSpace = surface_format_.colorSpace;
	swapchain_create_info.imageExtent = extent;
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 0;
	swapchain_create_info.pQueueFamilyIndices = nullptr;
	swapchain_create_info.preTransform = surface_capability_.currentTransform;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.presentMode = present_mode_;
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;
	
	VkResult status = vkCreateSwapchainKHR(logic_device_, &swapchain_create_info, nullptr, &swapchain_);
	if(status != VK_SUCCESS){
		return status;
	}
	swapchain_extent_ = extent;

	return VK_SUCCESS;
}

const VkResult VulkanRenderer::createDepthBuffer() noexcept(false){
	VkImageCreateInfo image_create_info;
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.pNext = nullptr;
	image_create_info.flags = 0;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = VK_FORMAT_D32_SFLOAT;
	image_create_info.extent.width = swapchain_extent_.width;
	image_create_info.extent.height = swapchain_extent_.height;
	image_create_info.extent.depth = 1;
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = 1;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_LINEAR;
	image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_create_info.queueFamilyIndexCount = 0;
	image_create_info.pQueueFamilyIndices = nullptr;
	image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkResult status = vkCreateImage(logic_device_, &image_create_info, nullptr, &depth_buffer_);
	if(status != VK_SUCCESS){
		return status;
	}

	VkMemoryRequirements memory_requirement;
	vkGetImageMemoryRequirements(logic_device_, depth_buffer_, &memory_requirement);

	VkMemoryAllocateInfo memory_allocate_info;
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.pNext = nullptr;
	memory_allocate_info.allocationSize = memory_requirement.size;
	memory_allocate_info.memoryTypeIndex = calcMemoryTypeIndex(memory_requirement.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	status = vkAllocateMemory(logic_device_, &memory_allocate_info, nullptr, &depth_buffer_memory_);
	if(status != VK_SUCCESS){
		return status;
	}
	status = vkBindImageMemory(logic_device_, depth_buffer_, depth_buffer_memory_, 0);
	if(status != VK_SUCCESS){
		return status;
	}

	return VK_SUCCESS;
}

const VkResult VulkanRenderer::createImageView() noexcept(false){
	uint32_t cnt = 0;
	VkResult status = vkGetSwapchainImagesKHR(logic_device_, swapchain_, &cnt, nullptr);
	if(status != VK_SUCCESS){
		return status;
	}
	swapchain_images_.resize(static_cast<size_t>(cnt));
	status = vkGetSwapchainImagesKHR(logic_device_, swapchain_, &cnt, swapchain_images_.data());
	if(status != VK_SUCCESS){
		return status;
	}
	swapchain_image_views_.resize(static_cast<size_t>(cnt));
	for(uint32_t i = 0; i < cnt; ++i){
		VkImageViewCreateInfo image_view_create_info;
		image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.pNext = nullptr;
		image_view_create_info.flags = 0;
		image_view_create_info.image = swapchain_images_[i];
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = surface_format_.format;
		image_view_create_info.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
		image_view_create_info.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
		
		status = vkCreateImageView(logic_device_, &image_view_create_info, nullptr, &swapchain_image_views_[i]);
		if(status != VK_SUCCESS){
			return status;
		}
	}

	VkImageViewCreateInfo image_view_create_info;
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.pNext = nullptr;
	image_view_create_info.flags = 0;
	image_view_create_info.image = depth_buffer_;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format = VK_FORMAT_D32_SFLOAT;
	image_view_create_info.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	image_view_create_info.subresourceRange = {VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1};

	return vkCreateImageView(logic_device_, &image_view_create_info, nullptr, &depth_buffer_view);
}

const VkResult VulkanRenderer::createRenderPass() noexcept(false){
	VkAttachmentDescription color_target;
	color_target.flags = 0;
	color_target.format = surface_format_.format;
	color_target.samples = VK_SAMPLE_COUNT_1_BIT;
	color_target.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_target.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_target.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_target.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_target.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_target.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_target;
	depth_target.flags = 0;
	depth_target.format = VK_FORMAT_D32_SFLOAT;
	depth_target.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_target.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_target.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_target.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_target.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_target.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_target.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	array<VkAttachmentDescription, 2> attachment_descriptions;
	attachment_descriptions[0] = color_target;
	attachment_descriptions[1] = depth_target;

	VkAttachmentReference color_reference;
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_reference;
	depth_reference.attachment = 1;
	depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description;
	subpass_description.flags = 0;
	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount = 0;
	subpass_description.pInputAttachments = nullptr;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_reference;
	subpass_description.pResolveAttachments = nullptr;
	subpass_description.pDepthStencilAttachment = &depth_reference;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.pPreserveAttachments = nullptr;

	VkRenderPassCreateInfo render_pass_create_info;
	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.pNext = nullptr;
	render_pass_create_info.flags = 0;
	render_pass_create_info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());
	render_pass_create_info.pAttachments = attachment_descriptions.data();
	render_pass_create_info.subpassCount = 1;
	render_pass_create_info.pSubpasses = &subpass_description;
	render_pass_create_info.dependencyCount = 0;
	render_pass_create_info.pDependencies = nullptr;

	return vkCreateRenderPass(logic_device_, &render_pass_create_info, nullptr, &render_pass_);
}

const VkResult VulkanRenderer::createFramebuffer() noexcept(false){
	array<VkImageView, 2> attachments;

	VkFramebufferCreateInfo framebuffer_create_info;
	framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_create_info.pNext = nullptr;
	framebuffer_create_info.flags = 0;
	framebuffer_create_info.renderPass = render_pass_;
	framebuffer_create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebuffer_create_info.pAttachments = attachments.data();
	framebuffer_create_info.width = swapchain_extent_.width;
	framebuffer_create_info.height = swapchain_extent_.height;
	framebuffer_create_info.layers = 1;

	framebuffers_.clear();
	for(auto& it : swapchain_image_views_){
		attachments[0] = it;
		attachments[1] = depth_buffer_view;

		VkFramebuffer new_framebuffer;
		VkResult status = vkCreateFramebuffer(logic_device_, &framebuffer_create_info, nullptr, &new_framebuffer);
		if(status != VK_SUCCESS){
			return status;
		}
		framebuffers_.push_back(new_framebuffer);
	}

	return VK_SUCCESS;
}

const VkResult VulkanRenderer::createCommandBuffer() noexcept(false){
	VkCommandBufferAllocateInfo comannd_buffer_allocate_info;
	comannd_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	comannd_buffer_allocate_info.pNext = nullptr;
	comannd_buffer_allocate_info.commandPool = command_pool_;
	comannd_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	comannd_buffer_allocate_info.commandBufferCount = static_cast<uint32_t>(swapchain_image_views_.size());

	command_buffers_.resize(static_cast<size_t>(comannd_buffer_allocate_info.commandBufferCount));
	VkResult status = vkAllocateCommandBuffers(logic_device_, &comannd_buffer_allocate_info, command_buffers_.data());
	if(status != VK_SUCCESS){
		return status;
	}

	fences_.resize(static_cast<size_t>(comannd_buffer_allocate_info.commandBufferCount));
	VkFenceCreateInfo fence_create_info;
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.pNext = nullptr;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for(auto& it : fences_){
		status = vkCreateFence(logic_device_, &fence_create_info, nullptr, &it);
		if(status != VK_SUCCESS){
			return status;
		}
	}
	return VK_SUCCESS;
}

const VkResult VulkanRenderer::createSemaphore() noexcept(false){
	VkSemaphoreCreateInfo semaphore_create_info;
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_create_info.pNext = nullptr;
	semaphore_create_info.flags = 0;
	VkResult status = vkCreateSemaphore(logic_device_, &semaphore_create_info, nullptr, &render_completed_semaphore_);
	if(status != VK_SUCCESS){
		return status;
	}
	return vkCreateSemaphore(logic_device_, &semaphore_create_info, nullptr, &present_completed_semaphore_);
}