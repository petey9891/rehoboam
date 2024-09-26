//#include <Rehoboam.h>
//#include "RehoboamCoords.h"
//
//Rehoboam::Rehoboam(Shader& shader, rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c)
//    : Runnable(m, c), renderer(), shader(shader), verticesBuffer(vertices, 36 * sizeof(float)), vcoordsBuffer(virtualCoords, 24 * sizeof(float))
//{
//    printf(">>> <Rehoboam> Initializing Rehoboam application\n");
//
//    buffer = (unsigned char*)malloc(PANEL_WIDTH * PANEL_HEIGHT * 3);
//
//    // Add an enable vertex attribute array and pointers
//    VertexBufferLayout verticesLayout;
//    verticesLayout.addFloat(shader.getAttribute("pos"), 3);
//    verticesBuffer.addLayout(verticesLayout);
//
//
//    VertexBufferLayout vcoordLayout;
//    vcoordLayout.addFloat(shader.getAttribute("coord"), 2);
//    vcoordsBuffer.addLayout(vcoordLayout);
//
//    printf(">>> <Rehoboam> Initialized Rehoboam application\n");
//}
//
//Rehoboam::~Rehoboam() {
//    free(this->buffer);
//}
//
//void Rehoboam::setInitialState() {
//    this->shader.setUniform1f("fade", 0.0f);
//}
//
//void Rehoboam::run() {
//    this->renderer.clear();
//
//    this->t += 0.01f;
//
//    this->shader.setUniform1f("time", this->t);
//    this->shader.setUniform1f("age", float(this->t - this->updateTime));
//    this->shader.setUniform1f("temperature", this->temperature);
//    this->shader.setUniform1fv("thread", this->cores, this->thread);
//
//    this->renderer.drawArrays(this->shader);
//
//    if (this->t < 5.0f && this->fadeLevel <= 1.0f) {
//        this->fadeLevel += 0.2f;
//        this->shader.setUniform1f("fade", this->fadeLevel);
//    }
//
//    if (this->temperature < 0.0f || this->temperature > 100.0f)
//        this->increment *= -1.0f;
//    this->temperature += increment;
//
//    GLCall(glFlush());
//    GLCall(glFinish());
//    GLCall(glReadPixels(0, 0, PANEL_WIDTH, PANEL_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer));
//
//    for (int x = 0; x < PANEL_WIDTH; x++) {
//        for (int y = 0; y < PANEL_HEIGHT; y++) {
//            int index = 3 * (x + y * PANEL_WIDTH);
//            this->canvas->SetPixel(x, y, buffer[index], buffer[index + 1], buffer[index + 2]);
//        }
//    }
//    this->canvas = this->matrix->SwapOnVSync(this->canvas);
//}

//#include <Config.h>
//
///*
//    Vertices of triangles per panel. Each panel contains two triangles
//*/
//static const GLfloat vertices[] = {
//    /* TOP PANEL */
//    -1.0f,              -1.0f,
//     0.0f,              -1.0f,
//     1.0f,               0.0f,
//
//    -0.33333333333f,    -1.0f,
//     0.0f,              -0.33333333333f,
//     1.0f,               0.0f,
//     /**********************/
//
//     /* BOTTOM RIGHT PANEL */
//     -0.33333333333f,    -1.0f,
//      0.0f,              -0.33333333333f,
//      1.0f,               0.0f,
//
//      0.33333333333f,    -1.0f,
//      0.0f,               0.33333333333f,
//      1.0f,               0.0f,
//      /**********************/
//
//      /* BOTTOM LEFT PANEL */
//      0.33333333333f,     -1.0f,
//      0.0f,                0.33333333333f,
//      1.0f,                0.0f,
//
//      1.0f,               -1.0f,
//      0.0f,                1.0f,
//      1.0f,                0.0f
//};
//
///*
// Each section denoted by comments represents an LED panel.
// The coordinates within each section (each row), represent the x and y location of the corners.
//
// If one were to draw a line between section coordinates, they would generate the panel's boundry, with one side missing.
//
// In each section, only the "left" and "right" side of the panels are defined. What might be the "top" of one panel, is technically the "left" of another.
// Because of this, the tops go undefined in each section, utilizing the coordinates of a differnt panel's side for their top. The commented out coordinates represent what is
// considered the "top" of each panel.
//
// With this, we can match the "virtual canvas" coordinates to each vertex to get a mapping of our canvas coordinates to the actual pixels on the panel array.
//*/
//static const GLfloat virtualCoords[] = {
//    // top = the fartest point from the viewer
//    // center = the point in the middle of all three screens
//    // middle = the line vertical splitting the bottom two panels and the horizontal line splitting the bottom two panels from the top
//    // bottom = the cloest point towards the viewer
//
//    /* BOTTOM LEFT SCREEN */
//     0.0f,       0.0f,    // center
//    -0.866f,     0.5f,    // left middle
//
//     0.0f,      -1.0f,    // middle bottom
//    -0.866f,    -0.5f,    // left bottom
//
//    //   0.0f,       0.0f,    // center
//    //   0.0f,      -1.0f,    // middle bottom
//        /**********************/
//
//        /* BOTTOM RIGHT SCREEN */
//         0.0f,       0.0f,    // center
//         0.0f,      -1.0f,    // middle bottom
//
//         0.866f,     0.5f,    // right middle
//         0.866f,    -0.5f,    // right bottom
//
//         //   0.0f,       0.0f,    // center
//         //   0.866f,     0.5f,    // right middle
//             /**********************/
//
//             /*     TOP SCREEN     */
//              0.0f,       0.0f,    // center
//              0.866f,     0.5f,    // right middle
//
//             -0.866f,     0.5f,    // left middle
//              0.0f,       1.0f,    // top
//
//              //   0.0f,       0.0f,    // center
//              //  -0.866f,     0.5f,    // left middle
//                  /**********************/
//};