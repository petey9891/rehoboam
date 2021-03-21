#include <Rehoboam.h>
#include "RehoboamCoords.h"

Rehoboam::Rehoboam(Shader& shader, rgb_matrix::FrameCanvas *c)
    : Runnable(c), renderer(), shader(shader), verticesBuffer(vertices, 36 * sizeof(float)), vcoordsBuffer(virtualCoords, 24 * sizeof(float)) 
{
    printf(">>> <Loading> Initializing Rehoboam application\n");

    buffer = (unsigned char*)malloc(PANEL_WIDTH * PANEL_HEIGHT * 3);

    // Add an enable vertex attribute array and pointers
    VertexBufferLayout verticesLayout;
    verticesLayout.addFloat(shader.getAttribute("pos"), 3);
    verticesBuffer.addLayout(verticesLayout);

    
    VertexBufferLayout vcoordLayout;
    vcoordLayout.addFloat(shader.getAttribute("coord"), 2);
    vcoordsBuffer.addLayout(vcoordLayout);

    printf(">>> <Loading> Initialized Rehoboam application\n");
}

Rehoboam::~Rehoboam() {
    free(this->buffer);
}

void Rehoboam::run() {
    this->renderer.clear();

    this->t += 0.01f;

    this->shader.setUniform1f("time", this->t);
    this->shader.setUniform1f("age", float(this->t - this->updateTime));
    this->shader.setUniform1f("temperature", this->temperature);
    this->shader.setUniform1fv("thread", this->cores, this->thread);

    this->renderer.drawArrays(this->shader);

    if (this->temperature < 0.0f || this->temperature > 100.0f)
        this->increment *= -1.0f;
    this->temperature += increment;

    glReadPixels(0, 0, PANEL_WIDTH, PANEL_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    for (int x = 0; x < PANEL_WIDTH; x++) {
        for (int y = 0; y < PANEL_HEIGHT; y++) {
            int index = 3*(x+y*PANEL_WIDTH);
            canvas->SetPixel(x, y, buffer[index], buffer[index+1], buffer[index+2]);
        }
    }
}