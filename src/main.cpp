#include <iostream>

#include <SocketServer.h>
#include <Command.h>
#include <Window.h>
#include <RGBMatrixConfig.h>
#include <Shader.h>

#include <Runnable.h>
#include <ColorPulse.h>
#include <Loading.h>
#include <Rehoboam.h>

int main(int argc, char* argv[]) {
    CubeWindow window;

    window.createEGLWindow();

    RGBMatrixConfig config;
    RGBMatrix* matrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &config.defaults, &config.runtime);
    FrameCanvas* canvas = matrix->CreateFrameCanvas();

    if (matrix == nullptr) {
        fprintf(stderr, "Error! Unable to create matrix!\n");
        return EXIT_FAILURE;
    }

    SocketServer server(60000);
    server.Start();
    server.HandleRequests();

    // Clear the whole screen (front buffer)
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create shaders
    Shader loadingShader("/home/pi/rehoboam/shaders/loading.shader");
    // Shader rehoboamShader("/home/pi/rehoboam/shaders/rehoboam.shader");
    
    // Create applications
    // Loading loading(loadingShader, canvas);
    Loading* loading = new Loading(loadingShader, matrix, canvas);
    // Rehoboam rehoboam(rehoboamShader, canvas);
    ColorPulse* pulse = new ColorPulse(matrix, canvas);

    // Bind the loading shader for the loading sequence
    // loadingShader.bind();

    Runnable* program = pulse;

    printf(">>> <Main> Running program\n");

    program->setInitialState();

    bool power = true;
    Command cmd;

    while (true) {
        if (server.hasCommands()) {
            Command nextCmd = server.getNextCommand();
            cmd = nextCmd;

            if (cmd == DisplayOn) {
                power = true;
            } else if (cmd == DisplayOff) {
                power = false;
            }
        }
        
        if (power) {
            program->run();
        } else {
            canvas->Fill(0, 0, 0);
            matrix->SwapOnVSync(canvas);
        }

        // if (loading.isDoneLoading && loading.shouldChangeScenes) {
        //     rehoboamShader.bind();
        //     program = fallback;
        //     program->setInitialState();
        //     loading.setSceneChangeIsFinished();
        // }
    }

    program->canvas->Clear();

    delete pulse;
    delete loading;

    server.Stop();
    window.destroy();

    return 0;
}