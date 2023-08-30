#pragma once

#ifndef _WIN32  // If not on Windows

#include "window.h"
#include <opengl/opengl_includes.h>

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

    int windowShouldClose() const override { return false; } // never closes

private:
    EGLWindow* m_window;
};

#endif  // !_WIN32