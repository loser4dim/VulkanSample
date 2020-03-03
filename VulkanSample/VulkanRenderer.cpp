#include "VulkanRenderer.hpp"
#include <vector>

using namespace std;

VulkanRenderer::VulkanRenderer() noexcept(false){

}

const bool VulkanRenderer::initializeVulkan(const char* const application_name, const char* const engine_name) noexcept(false){
	initializeInstance(application_name, engine_name);
	
	selectPhysicalDevice();

	graphics_queue_index_ = searchGraphicsQueueIndex();



#ifdef _DEBUG
	enableDebugReport();
#endif


	createLogicDevice();
	prepareCommnadPool();

	glfwCreateWindowSurface();
	selectSurfaceFormat(VK_FORMAT_B8G8R8A8_UNORM);
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR();

	VkBool32 is_supported;
	vkGetPhysicalDeviceSurfaceSupportKHR();

	createSwapChain();
	createDepthBuffer();
	createViews();
	createRenderPass();
	createFrameBuffer();

	prepareCommandBuffer();

	prepareSemaphore();

	prepare();



	return true;
}














const VkResult VulkanRenderer::initializeInstance(const char* const application_name, const char* const engine_name) noexcept(false){
	//Make Info for Application
	VkApplicationInfo application_info;
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName = application_name;
	application_info.pEngineName = engine_name;
	application_info.apiVersion = VK_API_VERSION_1_1;
	application_info.applicationVersion;
	application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	
	//Get Extension
	vector<const char*> extension_name;
	vector<VkExtensionProperties> extension_prop;
	uint32_t cnt = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &cnt, nullptr);
	extension_prop.resize(static_cast<size_t>(cnt));
	vkEnumerateInstanceExtensionProperties(nullptr, &cnt, extension_prop.data());
	for(const auto& it : extension_prop){
		extension_name.push_back(it.extensionName);
	}

	//Make Info for Creating Instance
	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extension_name.size());
	instance_create_info.ppEnabledExtensionNames = extension_name.data();
	instance_create_info.pApplicationInfo = &application_info;
#ifdef _DEBUG
	const char* const layer[] = {"VK_LAYER_LUNARG_standard_validation"};
	instance_create_info.enabledLayerCount = 1;
	instance_create_info.ppEnabledLayerNames = layer;
#endif

	//Create Instance
	VkResult ret = vkCreateInstance(&instance_create_info, nullptr, &instance_);
	return(ret);
}




















#ifdef _DEBUG
const bool Renderer::enableDebugReport() const noexcept(false){






}
#endif