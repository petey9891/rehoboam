#pragma once
#include "shader_application.h"

class CubeLoading : public ShaderApplication {
public:
    CubeLoading() {
        this->m_shader = std::make_unique<Shader>("shaders/main.vert.glsl", "shaders/lava-lamp/lava-lamp.frag.glsl");
        this->m_geometry = std::make_unique<ShaderGeometry>(true, false);
    }
};

//#pragma once
//
//#include <Runnable.h>
//
//#include <Config.h>
//#include <Renderer.h>
//#include <Shader.h>
//#include <VertexBuffer.h>
//
//class Loading : public Runnable {
//private:
//    float animstep = 0.01f;
//    float t = 0.0f;
//    float updateTime = -10.0f;
//    float fadeLevel = 1.0f;
//    float pulse = 0.02f;
//    float increment = 0.05f;
//
//    Renderer renderer;
//    Shader shader;
//    VertexBuffer verticesBuffer;
//    VertexBuffer vcoordsBuffer;
//
//    // Buffer of pixels
//    unsigned char* buffer;
//
//public:
//    Loading(Shader& shader, rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
//    virtual ~Loading();
//
//    void run() override;
//    void setInitialState() override;
//    void setSceneChangeIsFinished();
//
//public:
//    bool isDoneLoading = false;
//    bool shouldChangeScenes = false;
//};
