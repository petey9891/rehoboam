#version 100

varying vec3 color;

void main() {
    vec3 testColor = color;
    // gl_FragColor = vec4(0.2, 0.0, 0.0, 1.0);
    gl_FragColor = vec4(testColor 1.0);
}
