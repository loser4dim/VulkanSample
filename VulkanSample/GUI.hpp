#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3")

#include "Window.hpp"


class alignas(sizeof(float_t)) GUI final{
private:
	static constexpr const char* const WINDOW_TITLE_ = "Vulkan Sample - Y.Yabuki";

public:
	GUI() noexcept(false);
	GUI(const GUI&) = delete;
	GUI(GUI&&) = delete;
	GUI& operator=(const GUI&) = delete;
	GUI& operator=(GUI&&) = delete;
	~GUI() noexcept(false);

private:
	//std::unique_ptr<Monitor> primary_monitor_ = nullptr;
	//std::unique_ptr<Window> window_ = nullptr;
	//std::unique_ptr<Cursor> cursor_ = nullptr;

	

	int major_version_ = 0;
	int minor_varsion_ = 0;
	int revision_ = 0;

	bool is_supported_vulkan_ = false;

public:
	const bool isSupportedVulkan() const noexcept(false){
		return is_supported_vulkan_;
	}



private:
	const bool initializeGLFW() noexcept(false);




};