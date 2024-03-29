#version 100
attribute vec3 pos;
attribute vec2 coord;

// Background coord contains the virtual canvas coordinates.
// These coordinates are nto continuous anymore, 
//but can still be used by the fragment shader as they can be interpolated between the vertics.
varying vec2 backgroundCoord;

void main() {
    backgroundCoord = coord;
    gl_Position = vec4(pos, 1.0);
}
