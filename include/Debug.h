#pragma once

#include <Config.h>
#include <iostream>
#include <stdint.h>
#include <assert.h>

#define ASSERT(x) if (!(x)) assert(false)
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void inline GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool inline GLCheckError() {
    printf("Checking for errors...\n");
    while (GLenum error = glGetError()) { 
        printf("[OpenGL Error] ");
        switch(error) {
            case GL_INVALID_ENUM:
                printf("GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.");
                break;
            case GL_INVALID_VALUE:
                printf("GL_INVALID_OPERATION : A numeric argument is out of range.");
                break;
            case GL_INVALID_OPERATION:
                printf("GL_INVALID_OPERATION : The specified operation is not allowed in the current state.");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                printf("GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.");
                break;
            case GL_OUT_OF_MEMORY:
                printf("GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.");
                break;
            default:
                printf("Unrecognized error %d", error);
        }
          printf("\n");
          return false;
    }
    printf("no errors found\n");
    return true;
}


// #ifdef DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())
// #else
// #define GLCall(x) x
// #endif
