//#pragma once
//
//#include <Runnable.h>
//
//#include <Config.h>
//#include <Renderer.h>
//#include <Shader.h>
//#include <VertexBuffer.h>
//
//class Rehoboam : public Runnable {
//private:
//    float thread[8];
//    float animstep = 0.01f;
//    float t = 0.0f;
//    float updateTime = -10.0f;
//    float increment = 0.05f;
//    float temperature = 30.0f;
//    float fadeLevel = 0.0f;
//    int cores = 8;
//
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
//    Rehoboam(Shader& shader, rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
//    virtual ~Rehoboam();
//
//    void run() override;
//    void setInitialState() override;
//};
