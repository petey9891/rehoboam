#include <iostream>

#include <Command.h>
#include <Window.h>
#include <RGBMatrixConfig.h>
#include <Shader.h>

#include <Runnable.h>
#include <ColorPulse.h>
#include <Loading.h>
#include <Rehoboam.h>
#include <SolidColor.h>
#include <Christmas.h>

int main(int argc, char* argv[]) {
    CubeWindow window;
    window.createEGLWindow();

    RGBMatrixConfig config;
    RGBMatrix* matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &config.defaults, &config.runtime);
    FrameCanvas* canvas = matrix->CreateFrameCanvas();
    if (matrix == nullptr) {
        fprintf(stderr, "Error! Unable to create matrix!\n");
        return EXIT_FAILURE;
    }

    // Clear the whole screen (front buffer)
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Create shaders
    ColorPulse* pulse = new ColorPulse(matrix, canvas);

    Runnable* program = pulse;

    printf(">>> <Main> Running program\n");

    program->setInitialState();

    bool power = true;
    Command cmd;

    while (true) {
        if (power) {
            program->run();
        } else {
            canvas->Fill(0, 0, 0);
            matrix->SwapOnVSync(canvas);
        }
    }

    program->canvas->Clear();

    delete pulse;

    window.destroy();

    return 0;
}