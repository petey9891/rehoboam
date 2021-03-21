#include <Window.h>

static const EGLint eglConfigAttribs[] = {
	EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
	EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8,

	EGL_SAMPLE_BUFFERS, 1,
	EGL_SAMPLES, 4,

	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE};

// Width and height of the desired framebuffer
static const EGLint eglPbufferAttribs[] = {
	EGL_WIDTH,
	PANEL_WIDTH,
	EGL_HEIGHT,
	PANEL_HEIGHT,
	EGL_NONE,
};

static const EGLint eglContextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
										EGL_NONE};

void CubeWindow::createEGLWindow() {
    EGLDisplay display;
	int major;
	int minor;
	int desiredWidth;
	int desiredHeight;
    
    // Setup EGL
	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		fprintf(stderr, "Failed to get EGL display! Error: %d\n", eglGetError());
		return {};
	}

	if (eglInitialize(display, &major, &minor) == EGL_FALSE) {
		fprintf(stderr, "Failed to get EGL version! Error: %d\n", eglGetError());
		eglTerminate(display);
		return {};
	}

    printf("Initialized EGL version: %d.%d\n", major, minor);

	// EGL Config
	EGLint numConfigs;
	EGLConfig config;
	if (!eglChooseConfig(display, eglConfigAttribs, &config, 1, &numConfigs)) {
		fprintf(stderr, "Failed to get EGL Config! Error: %d\n", eglGetError());
		eglTerminate(display);
		return {};
	}

	// EGL Surface
	EGLSurface surface = eglCreatePbufferSurface(display, config, eglPbufferAttribs);
	if (surface == EGL_NO_SURFACE) {
		fprintf(stderr, "Failed to create EGL surface! Error: %d\n", eglGetError());
		eglTerminate(display);
		return {};
	}

	// Bind OpenGL API
	eglBindAPI(EGL_OPENGL_API);

    // EGL Context
	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, eglContextAttribs);
	if (context == EGL_NO_CONTEXT) {
		fprintf(stderr, "Failed to create EGL context! Error: %d\n", eglGetError());
		eglDestroySurface(display, surface);
		eglTerminate(display);
		return {};
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
		return {};
	}

	this->currentWindow = { display, surface, context };
}

void CubeWindow::destroy() {
	eglDestroyContext(this->currentWindow.display, this->currentWindow.context);
	eglDestroySurface(this->currentWindow.display, this->currentWindow.surface);
    eglTerminate(this->currentWindow.display);
}