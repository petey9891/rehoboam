#include <Loading.h>
#include "RehoboamCoords.h"

Loading::Loading(Shader& shader, rgb_matrix::FrameCanvas *c)
    : Runnable(c), renderer(), shader(shader), verticesBuffer(vertices, 36 * sizeof(float)), vcoordsBuffer(virtualCoords, 24 * sizeof(float)) 
{
    printf(">>> <Loading> Initializing Loading application\n");

    buffer = (unsigned char*)malloc(PANEL_WIDTH * PANEL_HEIGHT * 3);

    // Add an enable vertex attribute array and pointers
    VertexBufferLayout verticesLayout;
    verticesLayout.addFloat(shader.getAttribute("pos"), 3);
    verticesBuffer.addLayout(verticesLayout);

    
    VertexBufferLayout vcoordLayout;
    vcoordLayout.addFloat(shader.getAttribute("coord"), 2);
    vcoordsBuffer.addLayout(vcoordLayout);

    // Set initial state
    this->shader.setUniform1f("fade", 1.0f);

    printf(">>> <Loading> Initialized Loading application\n");
}

Loading::~Loading() {
    free(this->buffer);
}

void Loading::run() {
    float location = shader.getUniform("fade");
    float value;
    glGetUniform(0, location, 4, &value)
    printf("actual fade value: %f\n", value);

    this->renderer.clear();

    this->t += 0.25f;
    printf("fadeLevel %f\tt %f\n", this->fadeLevel, this->t);
    this->shader.setUniform1f("time", this->t);
    if (t > 300.0f) {
        this->shader.setUniform1f("fade", this->fadeLevel);
        this->fadeLevel -= 0.03f;

        if (this->fadeLevel <= 0.0f) {
            this->shader.setUniform1f("fade", 0.0f);
            this->isDoneLoading = true;
            this->shouldChangeScenes = true;
        }
    } else if (t > 242.0f) {
        this->shader.setUniform1f("fade", this->fadeLevel);
        float nextFadeValue = this->fadeLevel + this->pulse;
        if (nextFadeValue <= 0.00f || nextFadeValue > 1.00f) {
            this->pulse *= -1.0f;
            this->pulse += this->pulse > 0.0f ? 0.015f : -0.015f;
            t += 0.15f;
        }
        this->fadeLevel += this->pulse;
    }

    this->renderer.drawArrays(this->shader);

    glReadPixels(0, 0, PANEL_WIDTH, PANEL_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    for (int x = 0; x < PANEL_WIDTH; x++) {
        for (int y = 0; y < PANEL_HEIGHT; y++) {
            int index = 3*(x+y*PANEL_WIDTH);
            canvas->SetPixel(x, y, buffer[index], buffer[index+1], buffer[index+2]);
        }
    }
}

void Loading::setSceneChangeIsFinished() {
    this->isDoneLoading = false;
    this->shouldChangeScenes = false;
}