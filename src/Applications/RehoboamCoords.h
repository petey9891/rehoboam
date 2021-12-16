#include <Config.h>

/*
    Vertices of triangles per panel. Each panel contains two triangles
*/
// static const GLfloat vertices[] = {
//     /* TOP PANEL */
//     -1.0f, -1.0f, 0.0f,             0.6f, 0.0f, 0.0f,
//     -1.0f,  1.0f, 0.0f,             0.6f, 0.0f, 0.0f,
//     -0.33333333333f, -1.0f, 0.0f,   0.6f, 0.0f, 0.0f,
//     -0.33333333333f,  1.0f, 0.0f,   0.6f, 0.0f, 0.0f,
//     /**********************/
    
//     /* BOTTOM RIGHT PANEL */
//     -0.33333333333f, -1.0f, 0.0f,   0.0f, 0.6f, 0.0f,
//     -0.33333333333f,  1.0f, 0.0f,   0.0f, 0.6f, 0.0f,
//      0.33333333333f, -1.0f, 0.0f,   0.0f, 0.6f, 0.0f,
//      0.33333333333f,  1.0f, 0.0f,   0.0f, 0.6f, 0.0f,
//     /**********************/

//     /* BOTTOM LEFT PANEL */
//     0.33333333333f, -1.0f, 0.0f,    0.0f, 0.0f, 0.6f,
//     0.33333333333f,  1.0f, 0.0f,    0.0f, 0.0f, 0.6f,
//     1.0f, -1.0f, 0.0f,              0.0f, 0.0f, 0.6f,
//     1.0f,  1.0f, 0.0f,              0.0f, 0.0f, 0.6f,
// };
static const GLfloat vertices[] = {
    /* TOP PANEL */
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -0.33333333333f, -1.0f, 0.0f,
    -0.33333333333f,  1.0f, 0.0f,
    /**********************/
    
    /* BOTTOM RIGHT PANEL */
    -0.33333333333f, -1.0f, 0.0f,
    -0.33333333333f,  1.0f, 0.0f,
     0.33333333333f, -1.0f, 0.0f,
     0.33333333333f,  1.0f, 0.0f,
    /**********************/

    /* BOTTOM LEFT PANEL */
    0.33333333333f, -1.0f, 0.0f,
    0.33333333333f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 
    1.0f,  1.0f, 0.0f, 
};

/*
 Each section denoted by comments represents an LED panel.
 The coordinates within each section (each row), represent the x and y location of the corners.
 
 If one were to draw a line between section coordinates, they would generate the panel's boundry, with one side missing.
 
 In each section, only the "left" and "right" side of the panels are defined. What might be the "top" of one panel, is technically the "left" of another.
 Because of this, the tops go undefined in each section, utilizing the coordinates of a differnt panel's side for their top. The commented out coordinates represent what is
 considered the "top" of each panel.

 With this, we can match the "virtual canvas" coordinates to each vertex to get a mapping of our canvas coordinates to the actual pixels on the panel array.
*/
static const GLfloat virtualCoords[] = {
    // top = the fartest point from the viewer
    // center = the point in the middle of all three screens
    // middle = the line vertical splitting the bottom two panels and the horizontal line splitting the bottom two panels from the top
    // bottom = the cloest point towards the viewer

    /* BOTTOM LEFT SCREEN */
     0.0f,       0.0f,    // center
    -0.866f,     0.5f,    // left middle

     0.0f,      -1.0f,    // middle bottom
    -0.866f,    -0.5f,    // left bottom

//   0.0f,       0.0f,    // center
//   0.0f,      -1.0f,    // middle bottom
    /**********************/

    /* BOTTOM RIGHT SCREEN */
     0.0f,       0.0f,    // center
     0.0f,      -1.0f,    // middle bottom

     0.866f,     0.5f,    // right middle
     0.866f,    -0.5f,    // right bottom

//   0.0f,       0.0f,    // center
//   0.866f,     0.5f,    // right middle
    /**********************/

    /*     TOP SCREEN     */
     0.0f,       0.0f,    // center
     0.866f,     0.5f,    // right middle

    -0.866f,     0.5f,    // left middle
     0.0f,       1.0f,    // top

//   0.0f,       0.0f,    // center
//  -0.866f,     0.5f,    // left middle
    /**********************/
};