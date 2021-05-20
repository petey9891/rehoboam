#version 100
attribute vec3 pos;
attribute vec2 coord;

// Data to send from the vertex shader to the fragment shader
varying vec2 backgroundCoord;

void main() {
    backgroundCoord = coord;
    gl_Position = vec4(pos, 1.0);
}
