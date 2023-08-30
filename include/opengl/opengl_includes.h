// OpenGLIncludes.h
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _WIN32
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#else
#include "EGL/egl.h"
#include <GLES3/gl3.h>
#endif // _WIN32
