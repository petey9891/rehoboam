#include "led-matrix.h"

// Include EGL
#include <EGL/egl.h>
#include <GLES3/gl3.h>

// Include GLEW
// #include <GL/glew.h>

// Include GLFW
// #include <GLFW/glfw3.h>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Window.h"

#define DEBUG true

#include "Debug.h"

#define CORES 8

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace rgb_matrix;
using namespace std;

// Animation speed
float animstep = 0.01f;
float temperature = 30.0f;
float thread[CORES];
float tmpTemperature = 40.0f;
float tmpThread[CORES];

float t = 0.0f;
float updateTime = -10.0f;

static const GLfloat vertices[] = {
    -1.0f,
    -1.0f,
    0.0f,
    -1.0f,
    1.0f,
    0.0f,

    -0.33333333333f,
    -1.0f,
    0.0f,
    -0.33333333333f,
    1.0f,
    0.0f,

    -0.33333333333f,
    -1.0f,
    0.0f,
    -0.33333333333f,
    1.0f,
    0.0f,

    0.33333333333f,
    -1.0f,
    0.0f,
    0.33333333333f,
    1.0f,
    0.0f,

    0.33333333333f,
    -1.0f,
    0.0f,
    0.33333333333f,
    1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
};

static const GLfloat vcoords[] = {
    0.0f,
    0.0f,
    -0.866f,
    0.5f,

    0.0f,
    -1.0f,
    -0.866f,
    -0.5f,

    0.0f,
    0.0f,
    0.0f,
    -1.0f,

    0.866f,
    0.5f,
    0.866f,
    -0.5f,

    0.0f,
    0.0f,
    0.866f,
    0.5f,

    -0.866f,
    0.5f,
    0.0f,
    1.0f,
};

int main(int argc, char* argv[]) {
    CubeWindow window;
    EGLWindow win = window.createEGLWindow();

	// Clear the whole screen (front buffer)
	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    {
        Shader shader("../shaders/Rehoboam.shader");
        shader.bind();

	    GLuint vbo, vbocoord;
    	GLint posLoc, coordLoc, temperatureLoc, threadLoc, timeLoc, ageLoc, loadingLoc, fadeLoc;

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbocoord);
        glBindBuffer(GL_ARRAY_BUFFER, vbocoord);
        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vcoords, GL_STATIC_DRAW);

        // Get vertex attribute and uniform locations
        GLCall(posLoc = glGetAttribLocation(shader.m_RendererID, "pos"));
        GLCall(coordLoc = glGetAttribLocation(shader.m_RendererID, "coord"));
        GLCall(temperatureLoc = glGetUniformLocation(shader.m_RendererID, "temperature"));
        GLCall(threadLoc = glGetUniformLocation(shader.m_RendererID, "thread"));
        GLCall(timeLoc = glGetUniformLocation(shader.m_RendererID, "time"));
        GLCall(ageLoc = glGetUniformLocation(shader.m_RendererID, "age"));
        GLCall(loadingLoc = glGetUniformLocation(shader.m_RendererID, "loading"));
        GLCall(fadeLoc = glGetUniformLocation(shader.m_RendererID, "fade"));

        // Set our vertex data
        GLCall(glEnableVertexAttribArray(posLoc));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0));

        GLCall(glEnableVertexAttribArray(coordLoc));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbocoord));
        GLCall(glVertexAttribPointer(coordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0));

        // VertexBuffer verticesBuffer(vertices, 36 * sizeof(float));
        // VertexBuffer vcoordsBuffer(vcoords, 24 * sizeof(float));


        // VertexArray verticesArray;
        // VertexBufferLayout verticiesLayout;
        // verticiesLayout.addFloat(3);
        // verticesArray.addBuffer(verticesBuffer, verticiesLayout);

        // VertexArray vcoordsArray;
        // VertexBufferLayout vcoordsLayout;
        // vcoordsLayout.addFloat(2);
        // vcoordsArray.addBuffer(vcoordsBuffer, vcoordsLayout);

        // LED Matrix Settings
        RGBMatrix::Options defaults;
        rgb_matrix::RuntimeOptions runtime;

        defaults.hardware_mapping = "adafruit-hat-pwm";
        defaults.led_rgb_sequence = "BGR";
        defaults.pwm_bits = 11;
        defaults.pwm_lsb_nanoseconds = 50;
        // defaults.panel_type = "FM6126A";
        defaults.rows = 64;
        defaults.cols = 192;
        defaults.chain_length = 1;
        defaults.parallel = 1;
        // defaults.brightness = 100; // 60 is a good brightness for downtime

        runtime.drop_privileges = 0;
        runtime.gpio_slowdown = 1;

        // Create matrix
        RGBMatrix* matrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime);

        if (matrix == nullptr) {
            fprintf(stderr, "Error! Unable to create matrix!\n");
            return EXIT_FAILURE;
        }

        // Create canvas
        FrameCanvas* canvas = matrix->CreateFrameCanvas();

        // Buffer of pixels
	    unsigned char* buffer = (unsigned char*)malloc(W * H * 3);

        Renderer renderer;

        // Set initial state
        float fadeLevel = 1.0f;
        float pulse = 0.02f;
        float increment = 0.05f;

        bool loading = true;
        bool changingScene = false;

        GLCall(glUniform1f(loadingLoc, loading));
        GLCall(glUniform1f(fadeLoc, fadeLevel));
        while (true) {
            renderer.clear();
        
            t += loading ? 0.25f : 0.01f;

		    GLCall(glUniform1f(timeLoc, t));
		    GLCall(glUniform1f(ageLoc, float(t - updateTime)));
		    GLCall(glUniform1f(temperatureLoc, temperature));
		    GLCall(glUniform1fv(threadLoc, CORES, thread));
		    glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);


            if (loading) {
                 if (t > 100) {
                    // Fade loading screen
                    GLCall(glUniform1f(fadeLoc, fadeLevel));
                    fadeLevel -= 0.03f;

                    if (fadeLevel <= 0.0f) {
                        loading = false;
                        GLCall(glUniform1f(loadingLoc, false));
                        GLCall(glUniform1f(fadeLoc, 0.0f));
                        
                        // Need to fade into new scene
                        changingScene = true;
                    }
                } else if (t > 58) {
                    GLCall(glUniform1f(fadeLoc, fadeLevel));
                    float nextFadeValue = fadeLevel + pulse;
                    if (nextFadeValue <= 0.00f || nextFadeValue > 1.00f) {
                        pulse *= -1.0f;
                        pulse += pulse > 0.0f ? 0.015f : -0.015f;
                        t += 0.15f;
                    }
                    fadeLevel += pulse;
                }
            }

            if (changingScene) {
                GLCall(glUniform1f(fadeLoc, fadeLevel));
                fadeLevel += 0.03f;

                if (fadeLevel >= 1.0f) {
                    GLCall(glUniform1f(fadeLoc, 1.0f));
                    changingScene = false;
                }                
            }


            if (!loading && !changingScene) {
                // increment r
                if (temperature < 0.0f || temperature > 100.0f)
                    increment *= -1.0;
                temperature += increment;
            }


            // shader.bind();
            // shader.setUniform1f("time", t);
            // shader.setUniform1f("age", float(t - updateTime));

            // shader.setUniform1f("temperature", temperature);
            // // shader.setUniform1fv("thread", CORES, thread);
            // GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 12));
            // // renderer.drawArrays(verticesArray, shader);

            glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, buffer);

            for (int x = 0; x < W; x++) {
                for (int y = 0; y < H; y++) {
                    int index = 3*(x+y*W);
                    
                    // printf("rgb for pixel %d: rgb(%d, %d, %d)\n", x+y*W, buffer[index], buffer[index+1], buffer[index+2]);
                    // canvas->SetPixel(x, y, 255, 0, 255);
                    canvas->SetPixel(x, y, buffer[index], buffer[index+1], buffer[index+2]);
                }
            }
            canvas = matrix->SwapOnVSync(canvas);
        }

        free(buffer);
	    canvas->Clear();
    }

	// Close OpenGL window and terminate GLFW
	// glfwTerminate();

    eglDestroyContext(win.display, win.context);
	eglDestroySurface(win.display, win.surface);
    eglTerminate(win.display);

	return 0;
}