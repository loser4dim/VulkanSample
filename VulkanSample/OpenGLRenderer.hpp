#pragma once
#include "Renderer.hpp"

class alignas(sizeof(float_t)) OpenGLRenderer final : public Renderer{
public:
	OpenGLRenderer() noexcept = default;
	OpenGLRenderer(const OpenGLRenderer&) = delete;
	OpenGLRenderer(OpenGLRenderer&&) = delete;
	OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
	OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;
	~OpenGLRenderer() noexcept = default;

	void render() noexcept(false) override{
		return;
	}
};