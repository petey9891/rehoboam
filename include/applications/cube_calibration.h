#pragma once
#include "shader_application.h"

class CubeCalibration : public ShaderApplication {
public:
    CubeCalibration() {
        this->m_shader = std::make_unique<Shader>("shaders/main.vert.glsl", "shaders/calibration/calibration.frag.glsl");
        this->m_geometry = std::make_unique<ShaderGeometry>(true, false);
    }
};

