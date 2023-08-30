#pragma once

#include <memory>
#include <opengl/shader.h>
#include <opengl/shader_geometry.h>

class ShaderApplication {
public:
	virtual ~ShaderApplication() = default;

	virtual void activate() = 0;
	virtual void run() = 0;

protected:
	uint16_t m_frameCounter = 0;

	std::unique_ptr <Shader> m_shader;
	std::unique_ptr <ShaderGeometry> m_geometry;
};