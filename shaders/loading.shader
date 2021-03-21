#shader vertex
#version 100
attribute vec3 pos;
attribute vec2 coord;

// Data to send from the vertex shader to the fragment shader
varying vec2 backgroundCoord;

void main() {
    backgroundCoord = coord;
    gl_Position = vec4(pos, 1.0);
}

#shader fragment
#version 100
uniform float time;
uniform float loadingFade;

varying vec2 backgroundCoord;

// Rotates the circle by an angle
// angle - the angle to rotate the circle by
// note: https://en.wikipedia.org/wiki/Rotation_matrix
mat2 rotate2d(float angle) {
    return mat2(cos(angle), -sin(angle),
                sin(angle),  cos(angle));
}

// Creates a painted circle vec3. Adds a variation animation to the circle.
// uv - the coordinates of the circle
// rad - the circle radius
// width - the circle's stroke
vec3 circle (vec2 uv, float rad, float width) {
    float len = length(uv);
    float circle = smoothstep(rad-width, rad, len) - smoothstep(rad, rad+width, len);
    return vec3(circle);
}

void main() {
    // multiply by 0.5 to it centered in the cube
    vec2 uv = backgroundCoord.xy * 0.5;
    
    vec3 color;
    float radius = 0.25;

    // Create circle for gradient
    color = circle(uv, radius, 0.01);
    color += color;

    // Add rotation
    vec2 v = rotate2d(time) * uv; // matrix multiplication to map rotated coordinates with original coordinates
    color *= vec3(v.x, v.y, 0.475-(v.x));

    // color *= vec3(loadingFade);
    
	gl_FragColor = vec4(color, 1.0);
}
