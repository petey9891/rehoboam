//
// Created by Andrew Peterson on 11/2/23.
//

#pragma once
#include "shader_application.h"

class Water : public ShaderApplication {
public:
    Water() {
        this->m_shader = std::make_unique<Shader>("shaders/main.vert.glsl", "shaders/water/water.frag.glsl");
        this->m_geometry = std::make_unique<ShaderGeometry>(true, false);
    }
};
