#include <iostream>

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

    // Clear the whole screen (front buffer)
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create shaders
    Shader loadingShader("/home/pi/rehoboam/shaders/loading.shader");
    Shader rehoboamShader("/home/pi/rehoboam/shaders/rehoboam.shader");
    
    // Create applications
    Loading loading(loadingShader, canvas);
    Rehoboam rehoboam(rehoboamShader, canvas);
    ColorPulse pulse(canvas);

    // Bind the loading shader for the loading sequence
    rehoboamShader.bind();

    Runnable* program = &loading;
    Runnable* fallback = &rehoboam;

    printf(">>> <Main> Running program\n");
    while (true) {
        program->run();

        if (loading.isDoneLoading && loading.shouldChangeScenes) {
            program = fallback;
            loading.setSceneChangeIsFinished();
        }

        canvas = matrix->SwapOnVSync(canvas);
    }

    canvas->Clear();

    window.destroy();

    return 0;
}

// #include "led-matrix.h"

// // Include EGL
// #include <EGL/egl.h>
// #include <GLES3/gl3.h>

// // Include GLEW
// // #include <GL/glew.h>

// // Include GLFW
// // #include <GLFW/glfw3.h>

// #include "VertexBuffer.h"
// #include "VertexArray.h"
// #include "IndexBuffer.h"
// #include "Shader.h"
// #include "Renderer.h"
// #include "Window.h"

// #define DEBUG true

// #include "Debug.h"

// #define CORES 8

// using rgb_matrix::RGBMatrix;
// using rgb_matrix::Canvas;

// using namespace rgb_matrix;
// using namespace std;

// // Animation speed
// float animstep = 0.01f;
// float temperature = 30.0f;
// float thread[CORES];
// float tmpTemperature = 40.0f;
// float tmpThread[CORES];

// float t = 0.0f;
// float updateTime = -10.0f;

// /*
//     Vertices of triangles per panel. Each panel contains two triangles
// */
// static const GLfloat vertices[] = {
//     /* TOP PANEL */
//     -1.0f,              -1.0f,
//      0.0f,              -1.0f,
//      1.0f,               0.0f,

//     -0.33333333333f,    -1.0f,
//      0.0f,              -0.33333333333f,
//      1.0f,               0.0f,
//     /**********************/
    
//     /* BOTTOM RIGHT PANEL */
//     -0.33333333333f,    -1.0f,
//      0.0f,              -0.33333333333f,    
//      1.0f,               0.0f,

//      0.33333333333f,    -1.0f,
//      0.0f,               0.33333333333f,
//      1.0f,               0.0f,
//     /**********************/

//     /* BOTTOM LEFT PANEL */
//     0.33333333333f,     -1.0f,
//     0.0f,                0.33333333333f,
//     1.0f,                0.0f,

//     1.0f,               -1.0f,
//     0.0f,                1.0f,
//     1.0f,                0.0f
// };

// /*
//  Each section denoted by comments represents an LED panel.
//  The coordinates within each section (each row), represent the x and y location of the corners.
 
//  If one were to draw a line between section coordinates, they would generate the panel's boundry, with one side missing.
 
//  In each section, only the "left" and "right" side of the panels are defined. What might be the "top" of one panel, is technically the "left" of another.
//  Because of this, the tops go undefined in each section, utilizing the coordinates of a differnt panel's side for their top. The commented out coordinates represent what is
//  considered the "top" of each panel.

//  With this, we can match the "virtual canvas" coordinates to each vertex to get a mapping of our canvas coordinates to the actual pixels on the panel array.
// */
// static const GLfloat virtualCoords[] = {
//     // top = the fartest point from the viewer
//     // center = the point in the middle of all three screens
//     // middle = the line vertical splitting the bottom two panels and the horizontal line splitting the bottom two panels from the top
//     // bottom = the cloest point towards the viewer

//     /* BOTTOM LEFT SCREEN */
//      0.0f,       0.0f,    // center
//     -0.866f,     0.5f,    // left middle

//      0.0f,      -1.0f,    // middle bottom
//     -0.866f,    -0.5f,    // left bottom

// //   0.0f,       0.0f,    // center
// //   0.0f,      -1.0f,    // middle bottom
//     /**********************/

//     /* BOTTOM RIGHT SCREEN */
//      0.0f,       0.0f,    // center
//      0.0f,      -1.0f,    // middle bottom

//      0.866f,     0.5f,    // right middle
//      0.866f,    -0.5f,    // right bottom

// //   0.0f,       0.0f,    // center
// //   0.866f,     0.5f,    // right middle
//     /**********************/

//     /*     TOP SCREEN     */
//      0.0f,       0.0f,    // center
//      0.866f,     0.5f,    // right middle

//     -0.866f,     0.5f,    // left middle
//      0.0f,       1.0f,    // top

// //   0.0f,       0.0f,    // center
// //  -0.866f,     0.5f,    // left middle
//     /**********************/
// };

// int main(int argc, char* argv[]) {
//     CubeWindow window;
//     EGLWindow win = window.createEGLWindow();

// 	// Clear the whole screen (front buffer)
// 	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//     {
//         // Create shader
//         Shader shader("/home/pi/rehoboam/shaders/rehoboam.shader");
//         shader.bind();

//         // Generate buffers and populate with data
//         VertexBuffer verticesBuffer(vertices, 36 * sizeof(float));
//         VertexBuffer vcoordsBuffer(virtualCoords, 24 * sizeof(float));

//         // Add an enable vertex attribute array and pointers
//         VertexBufferLayout verticesLayout;
//         verticesLayout.addFloat(shader.getAttribute("pos"), 3);
//         verticesBuffer.addLayout(verticesLayout);

//         VertexBufferLayout vcoordLayout;
//         vcoordLayout.addFloat(shader.getAttribute("coord"), 2);
//         vcoordsBuffer.addLayout(vcoordLayout);

//         // LED Matrix Settings
//         RGBMatrix::Options defaults;
//         rgb_matrix::RuntimeOptions runtime;

//         defaults.hardware_mapping = "adafruit-hat-pwm";
//         defaults.led_rgb_sequence = "BGR";
//         defaults.pwm_bits = 11;
//         defaults.pwm_lsb_nanoseconds = 50;
//         // defaults.panel_type = "FM6126A";
//         defaults.rows = 64;
//         defaults.cols = 192;
//         defaults.chain_length = 1;
//         defaults.parallel = 1;
//         // defaults.brightness = 100; // 60 is a good brightness for downtime

//         runtime.drop_privileges = 0;
//         runtime.gpio_slowdown = 1;

//         // Create matrix
//         RGBMatrix* matrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime);

//         if (matrix == nullptr) {
//             fprintf(stderr, "Error! Unable to create matrix!\n");
//             return EXIT_FAILURE;
//         }

//         // Create canvas
//         FrameCanvas* canvas = matrix->CreateFrameCanvas();

//         // Buffer of pixels
// 	    unsigned char* buffer = (unsigned char*)malloc(W * H * 3);

//         Renderer renderer;

//         // Set initial state
//         float fadeLevel = 1.0f;
//         float pulse = 0.02f;
//         float increment = 0.05f;

//         bool loading = true;
//         bool changingScene = false;

//         shader.setUniform1f("loading", loading);
//         shader.setUniform1f("fade", fadeLevel);
//         while (true) {
//             renderer.clear();
        
//             t += loading ? 0.25f : 0.01f;

//             shader.setUniform1f("time", t);
//             shader.setUniform1f("age", float(t - updateTime));
//             shader.setUniform1f("temperature", temperature);
//             shader.setUniform1fv("thread", CORES, thread);

//             if (loading) {
//                  if (t > 200) {
//                     // Fade loading screen
//                     shader.setUniform1f("fade", fadeLevel);
//                     fadeLevel -= 0.03f;

//                     if (fadeLevel <= 0.0f) {
//                         loading = false;
//                         shader.setUniform1f("loading", false);
//                         shader.setUniform1f("fade", 0.0f);
                        
//                         // Need to fade into new scene
//                         changingScene = true;
//                     }
//                 } else if (t > 158) {
//                     shader.setUniform1f("fade", fadeLevel);
//                     float nextFadeValue = fadeLevel + pulse;
//                     if (nextFadeValue <= 0.00f || nextFadeValue > 1.00f) {
//                         pulse *= -1.0f;
//                         pulse += pulse > 0.0f ? 0.015f : -0.015f;
//                         t += 0.15f;
//                     }
//                     fadeLevel += pulse;
//                 }
//             }

//             if (changingScene) {
//                     shader.setUniform1f("fade", fadeLevel);
//                 fadeLevel += 0.03f;

//                 if (fadeLevel >= 1.0f) {
//                     shader.setUniform1f("fade", 1.0f);
//                     changingScene = false;
//                 }                
//             }

//             if (!loading && !changingScene) {
//                 // increment r
//                 if (temperature < 0.0f || temperature > 100.0f)
//                     increment *= -1.0;
//                 temperature += increment;
//             }

//             renderer.drawArrays(shader);

//             glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, buffer);

//             for (int x = 0; x < W; x++) {
//                 for (int y = 0; y < H; y++) {
//                     int index = 3*(x+y*W);
//                     canvas->SetPixel(x, y, buffer[index], buffer[index+1], buffer[index+2]);
//                 }
//             }
//             canvas = matrix->SwapOnVSync(canvas);
//         }

//         free(buffer);
// 	    canvas->Clear();
//     }

// 	// Close OpenGL window and terminate GLFW
    // eglDestroyContext(win.display, win.context);
	// eglDestroySurface(win.display, win.surface);
    // eglTerminate(win.display);

// 	return 0;
// }