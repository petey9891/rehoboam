#version 460 core

in vec2 fragCoord;

out vec4 fragColor;

uniform vec3 iResolution; // Declare the resolution uniform
uniform float iTime;      // Declare the time uniform



void main() {
    vec2 scaledFragCoord = fragCoord * iResolution.xy; // Use the interpolated tex-coord to get fragCoord
    vec2 uv = (scaledFragCoord * 2.0 - iResolution.xy) / iResolution.y + vec2(-0.0, -0.0);

    vec3 pos = vec3((uv - 0.5) * 2.0 * 0.7, iTime * 2.0);
    vec3 dir = normalize(vec3(pos.xy,3));
    pos.xy += (3.0 * 3.14159265 / 4.0) * vec2(sin(pos.z * 0.2), cos(pos.z * 0.2)) - vec2(3.14159265 / 2.0, 0);
    for(int i = 0; i < 164; i++) {
        float d = max(abs(sin(pos.x) * cos(pos.y) + sin(pos.y) * cos(pos.z * 0.2) + sin(pos.z * 0.2) * cos(pos.x)) + 0.0001, 0.0);
        pos += dir * d * 2.0;
    }
    float c = 16.0 / (pos.z - iTime * 2.0 + 16.0);
    fragColor = vec4(0.1, 0.7, 1, 1) * c + vec4(0,0.1,0.1,1) * (1.0 - c);
}