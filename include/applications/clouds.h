//
// Created by Andrew Peterson on 11/2/23.
//

#pragma once
#include "shader_application.h"

class Clouds : public ShaderApplication {
public:
    Clouds() {
        this->m_shader = std::make_unique<Shader>("shaders/main.vert.glsl", "shaders/clouds/clouds.frag.glsl");
        this->m_geometry = std::make_unique<ShaderGeometry>(true, false);
    }
};
