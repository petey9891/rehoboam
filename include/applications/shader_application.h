#pragma once

#include <memory>
#include <opengl/shader.h>
#include <opengl/shader_geometry.h>

// Forward declare
class ApplicationState;

class ShaderApplication {
public:
    ShaderApplication();
    ~ShaderApplication();

    void activate();
    void run(const ApplicationState::State& state);

protected:
	uint16_t m_frameCounter = 0;

	std::unique_ptr <Shader> m_shader;
	std::unique_ptr <ShaderGeometry> m_geometry;
};