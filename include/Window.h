#include <Config.h>
#include <iostream>

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
