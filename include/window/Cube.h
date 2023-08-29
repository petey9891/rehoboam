#pragma once
#include "Window.h"

#if defined(__APPLE__) || defined(__linux__)
#include "EGL/egl.h"
#include <GLES3/gl3.h>
#elif defined(WIN32)
using EGLDisplay = void*;
using EGLSurface = void*;
using EGLContext = void*;
#endif

struct EGLWindow {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
};

class Cube : public Window<EGLWindow> {
public:
    Cube(int width, int height);
    virtual ~Cube();

    virtual void use() override;
    virtual void unuse() override;
};
