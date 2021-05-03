#pragma once

#include <Config.h>
#include <iostream>

struct EGLWindow {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
};

class CubeWindow {
private:
    EGLWindow currentWindow;

public:
    CubeWindow() = default;
    ~CubeWindow() = default;

    void createEGLWindow();
    void destroy();
};
