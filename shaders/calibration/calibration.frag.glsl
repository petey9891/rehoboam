#version 410 core

in vec2 fragCoord;

out vec4 fragColor;

uniform vec3 iResolution; // Declare the resolution uniform
uniform float iTime;      // Declare the time uniform
uniform int iFrame;       // Declare the frame uniform

void main()
{
    vec2 scaledFragCoord = fragCoord * iResolution.xy; // Use the interpolated tex-coord to get fragCoord
    vec2 uv = (scaledFragCoord * 2.0 - iResolution.xy) / iResolution.y + vec2(-0.0, -0.0);

    fragColor = vec4(fragCoord, 0.0, 1.0);
}