#include <iostream>
#include <memory>

#include <network/command_message_client.h>
#include <state/application_state.h>
#include <window/window.h>
#include <window/cube.h>
#include <window/virtual_cube.h>
#include <applications/shader_application.h>
#include <applications/cube_calibration.h>

int main(int argc, char* argv[]) {
    std::unique_ptr<Window> window = nullptr;
    
    #if !defined(__APPLE__) && !defined(__linux__)
    std::make_unique<Cube>(192, 64);
    #else
    window = std::make_unique<VirtualCube>(1200, 1200, "Rehoboam Sandbox");
    #endif  // !_WIN32 and !__APPLE__

    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }


    std::unique_ptr<ApplicationState> state = std::make_unique<ApplicationState>();
    std::unique_ptr<CommandMessageClient> client = std::make_unique<CommandMessageClient>();

    client->initialize();

    while (!window->windowShouldClose()) {
        client->processNextCommand(*state);

        window->use();
        
        auto activeApp = state->getActiveShaderApplication();
        if (activeApp) {
            activeApp->run(state->getState());
        }

        window->unuse();
    }

    //RGBMatrixConfig config;
    //RGBMatrix* matrix = RGBMatrix::CreateFromFlags(&argc, &argv, &config.defaults, &config.runtime);
    //FrameCanvas* canvas = matrix->CreateFrameCanvas();
    //if (matrix == nullptr) {
    //    fprintf(stderr, "Error! Unable to create matrix!\n");
    //    return EXIT_FAILURE;
    //}


    //// Create applications
    //Loading* loading = new Loading(loadingShader, matrix, canvas);
    //// Rehoboam* rehoboam = new Rehoboam(rehoboamShader, matrix, canvas);
    //ColorPulse* pulse = new ColorPulse(matrix, canvas);
    //SolidColor* solid = new SolidColor(matrix, canvas);
    //Christmas* christmas = new Christmas(matrix, canvas);

    //// Bind the loading shader for the loading sequence
    //// loadingShader.bind();

    //// Runnable* program = loading;
    //Runnable* program = pulse;
    //// Runnable* program = christmas;
    //// Runnable* program = rehoboam;
    //// Runnable* fallback = pulse;
    //// Runnable* fallback = rehoboam;

    //printf(">>> <Main> Running program\n");

    //program->setInitialState();

    //bool power = true;
    //Command cmd;

    //while (true) {
    //    if (client.hasCommands()) {
    //        Command nextCmd = client.getNextCommand();
    //        cmd = nextCmd;

    //        switch (cmd.type) {
    //            case DisplayOn:
    //                power = true;
    //                break;
    //            case DisplayOff:
    //                power = false;
    //                break;
    //            case Brightness:
    //                program->setCommand(cmd);
    //                break;
    //            case ColorPulseMode:
    //                program = pulse;
    //                break;
    //            case RehoboamMode:
    //                // rehoboamShader.bind();
    //                // program = rehoboam;
    //                break;
    //            case StaticColor:
    //                program = solid;
    //                program->setCommand(cmd);
    //                break;
    //            case ChristmasMode:
    //                program = christmas;
    //                break;
    //        }
    //    }

    //    if (power) {
    //        program->run();
    //    } else {
    //        canvas->Fill(0, 0, 0);
    //        matrix->SwapOnVSync(canvas);
    //    }

    //     if (loading->isDoneLoading && loading->shouldChangeScenes) {
    //         rehoboamShader.bind();
    //         program = fallback;
    //         program->setInitialState();
    //         loading->setSceneChangeIsFinished();
    //     }
    //}

    //program->canvas->Clear();

    return 0;
}