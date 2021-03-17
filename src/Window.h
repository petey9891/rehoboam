#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <iostream>

//Resolution, three panels with 64x64 each.
#define W 192
#define H 64

struct EGLWindow {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
};

class CubeWindow {
    public:
        CubeWindow() = default;
        ~CubeWindow() = default;

        EGLWindow createEGLWindow();
};
