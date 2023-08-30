#version 460 core

in vec2 fragCoord;

out vec4 fragColor;

uniform vec3 iResolution; // Declare the resolution uniform
uniform float iTime;      // Declare the time uniform
uniform int iFrame;       // Declare the frame uniform

const int CORES = 8;
const float T1 = 40.0;
const float T2 = 60.0;
const float T3 = 80.0;
uniform float temperature;
uniform float thread[CORES];

vec3 bColor = vec3(0.1, 0.6, 0.4);
vec3 bColorWarm = vec3(0.5, 0.5, 0.1);
vec3 bColorHot = vec3(0.6, 0.2, 0.1);

vec3 rColor = vec3(0.7, 1.0, 0.9);
vec3 rColorWarm = vec3(1.0, 1.0, 0.6);
vec3 rColorHot = vec3(1.0, 1.0, 1.0);

uniform float age;
uniform float fade;

float phi;
float threadf = 0.0;

// Adds the animation for the circle moving back and forth
// v1 - the coordinates of circle
// v2 - the boundtry in which the circle can move away from its coordinates?
// strength - the number of distortions on the circle
// speed - the speed of the animation
float variance(float normalizedCoord, float strength, float speed) {
	return sin(normalizedCoord * strength + iTime * speed) / 100.0;
}

// Creates circle float. Adds a variance animation to the circle.
// uv - the coordinates of the circle
// rad - the circle radius
// width - the circle's stroke
float circle(vec2 uv, float rad, float width) {
    float strength = 5.0;
    float speed = 2.0;

    float frame = length(uv);
    vec2 normalizedCoords = normalize(uv);
    // adds variance to each half of the circle by providing normalized x and y coords
    frame += variance(normalizedCoords.y, strength, speed) - variance(normalizedCoords.x, strength, speed);

    // Multiply by threadf to enlarge one portion of the circle
    float frameWidth = width + width*threadf*0.1;

    return smoothstep(rad-frameWidth, rad, frame) - smoothstep(rad, rad+frameWidth, frame);
}


void main()
{
    vec2 scaledFragCoord = fragCoord * iResolution.xy; // Use the interpolated tex-coord to get fragCoord
    vec2 uv = (scaledFragCoord * 2.0 - iResolution.xy) / iResolution.y + vec2(-0.0, -0.0);

    vec2 coords = uv.xy*0.5;
    float radius = 0.25;

    float phi = (atan(coords.y, coords.x)+3.1415926538)/3.1415926538*float(CORES)*0.5;
    vec2 background = uv.xy * 0.5 * 10.0 - vec2(19.0);
    vec2 i = background;
    float c = 1.0;
    float inten = 0.05;

    for (int n = 0; n < 8; n++) {
        float t = iTime * (0.7 - (0.2 / float(n+1)));
        i = background + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
        c += 1.0 / length(vec2(background.x / (2.0 * sin(i.x + t) / inten), background.y / (cos(i.y + t) / inten)));
    }

    c /= 8.0;
    c = 1.5 - sqrt(pow(c, 2.0));

    bColor.g = clamp(coords.x, 0.0, 1.0);
    bColor = smoothstep(T2, T1, temperature)*bColor + smoothstep(T1, T2, temperature)*smoothstep(T3, T2, temperature)*bColorWarm + smoothstep(T2, T3, temperature)*bColorHot;

    rColor = smoothstep(50.0, 0.0, threadf)*rColor + smoothstep(0.0, 50.0, threadf)*smoothstep(100.0, 50.0, threadf)*rColorWarm + smoothstep(50.0, 100.0, threadf)*rColorHot;

    rColor *= circle(coords, radius, 0.01);

    // multiplying c increases wave intensity
    vec3 outColor = bColor * c * c * c * c + rColor;

    float coreIndex = 0.0;
    for (int i = 0; i < CORES; i++) {
        threadf += clamp(1.0-abs(phi-coreIndex), 0.0, 1.0)*thread[i];
        coreIndex += 1.0;
    }

//    outColor *= vec3(fade);

    fragColor = vec4(mix(outColor, outColor, smoothstep(5.0, 10.0, age)), 1.0);
}