#version 410 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

out vec2 fragCoord; 

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
    fragCoord = inUV;

    // Output position of the vertex, in clip space
    gl_Position =  MVP * vec4(inPosition, 1.0);
}
