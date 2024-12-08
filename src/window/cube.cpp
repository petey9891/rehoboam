#if !defined(_WIN32) && !defined(__APPLE__)

#include <iostream>
#include <window/cube.h>

static const EGLint eglConfigAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_SAMPLE_BUFFERS, 1,
    EGL_SAMPLES, 4,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_NONE
};

// Width and height of the desired framebuffer
static const EGLint eglPbufferAttribs[] = {
    EGL_WIDTH, PANEL_WIDTH,
    EGL_HEIGHT, PANEL_HEIGHT,
    EGL_NONE
};

static const EGLint eglContextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};


Cube::Cube(int width, int height) : Window(width, height) {
    EGLDisplay display;
    int major;
    int minor;
    int desiredWidth;
    int desiredHeight;

    // Setup EGL
    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        std::cerr << "Failed to get EGL display " << eglGetError() << std::endl;
        return;
    }

    if (eglInitialize(display, &major, &minor) == EGL_FALSE) {
        std::cerr << "Failed to get EGL version " << eglGetError() << std::endl;

        eglTerminate(display);
        return;
    }

    // EGL Config
    EGLint numConfigs;
    EGLConfig config;
    if (!eglChooseConfig(display, eglConfigAttribs, &config, 1, &numConfigs)) {
        std::cerr << "Failed to get EGL Config " << eglGetError() << std::endl;

        eglTerminate(display);
        return;
    }

    // EGL Surface
    EGLSurface surface = eglCreatePbufferSurface(display, config, eglPbufferAttribs);
    if (surface == EGL_NO_SURFACE) {
        std::cerr << "Failed to create EGL surface " << eglGetError() << std::endl;

        eglTerminate(display);
        return;
    }

    // Bind OpenGL API
    eglBindAPI(EGL_OPENGL_API);

    // EGL Context
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, eglContextAttribs);
    if (context == EGL_NO_CONTEXT) {
        std::cerr << "Failed to create EGL context " << eglGetError() << std::endl;

        eglDestroySurface(display, surface);
        eglTerminate(display);
        return;
    }

    eglMakeCurrent(display, surface, surface, context);

    desiredWidth = eglPbufferAttribs[1];
    desiredHeight = eglPbufferAttribs[3];

    glViewport(0, 0, desiredWidth, desiredHeight);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    printf("GL Viewport size: %dx%d\n", viewport[2], viewport[3]);

    if (desiredWidth != viewport[2] || desiredHeight != viewport[3]) {
        fprintf(stderr, "Error! the glViewport/glgetIntergerv are not working! EGL might be faulty!\n");
        std::cerr << "Error! the glViewport/glgetIntergerv are not working! EGL might be faulty! " << eglGetError() << std::endl;

        return;
    }

    this->m_window = { display, surface, context };
}

Cube::~Cube() {
    eglDestroyContext(this->m_window->display, this->m_window->context);
    eglDestroySurface(this->m_window->display, this->m_window->surface);
    eglTerminate(this->m_window->display);
}

void Cube::use() {

}

void Cube::unuse() {

}

#endif  // !_WIN32 and !__APPLE__