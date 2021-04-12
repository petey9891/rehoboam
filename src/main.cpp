#include <iostream>

#include <Server.h>
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

    Server server(60000);
    server.Start();
    server.HandleRequests();

    // Clear the whole screen (front buffer)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create shaders
    Shader loadingShader("/home/pi/rehoboam/shaders/loading.shader");
    Shader rehoboamShader("/home/pi/rehoboam/shaders/rehoboam.shader");
    
    // Create applications
    Loading* loading = new Loading(loadingShader, matrix, canvas);
    Rehoboam* rehoboam = new Rehoboam(rehoboamShader, matrix, canvas);
    ColorPulse* pulse = new ColorPulse(matrix, canvas);

    // Bind the loading shader for the loading sequence
    loadingShader.bind();

    Runnable* program = loading;
    Runnable* fallback = rehoboam;

    printf(">>> <Main> Running program\n");

    program->setInitialState();

    bool power = true;
    Command cmd;

    while (true) {
        if (server.hasCommands()) {
            Command nextCmd = server.getNextCommand();
            cmd = nextCmd;

            switch (cmd.type) {
                case DisplayOn:
                    power = true;
                    break;
                case DisplayOff:
                    power = false;
                    break;
                case Brightness:
                    program->setCommand(cmd);
                    break;
                case ColorPulse:
                    program = pulse;
                    break;
                case Rehoboam:
                    rehoboamShader.bind();
                    program = rehoboam;
                    program->setInitialState();
                    break;
            }
        }

        if (power) {
            program->run();
        } else {
            canvas->Fill(0, 0, 0);
            matrix->SwapOnVSync(canvas);
        }

        if (loading->isDoneLoading && loading->shouldChangeScenes) {
            rehoboamShader.bind();
            program = fallback;
            program->setInitialState();
            loading->setSceneChangeIsFinished();
        }
    }

    program->canvas->Clear();

    delete pulse;
    delete loading;
    delete rehoboam;

    // server.Stop();
    window.destroy();

    return 0;
}