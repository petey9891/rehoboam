#version 460 core

in vec2 fragCoord;

out vec4 fragColor;

uniform vec3 iResolution; // Declare the resolution uniform
uniform float iTime;      // Declare the time uniform
uniform int iFrame;       // Declare the frame uniform


#define GS 10.
#define SMH 20./iResolution.x
#define PI 3.14159265359

float hexdist(vec2 uv){
    vec2 p = abs(uv);
    return max(
    	p.x,
        dot(p, normalize(vec2(1, 1.73)))
        );    
}

vec2 hash( vec2 p ) {
	p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}


float noise( in vec2 p ) {
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
	vec2 i = floor(p + (p.x+p.y)*K1);	
    vec2 a = p - i + (i.x+i.y)*K2;
    vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
    vec2 b = a - o + K2;
	vec2 c = a - 1.0 + 2.0*K2;
    vec3 h = max(0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot(n, vec3(70.0));	
}


float R1(vec2 uv){
    return noise(uv);
}

void main()
{
    vec2 scaledFragCoord = fragCoord * iResolution.xy; // Use the interpolated tex-coord to get fragCoord
    vec2 uv = (scaledFragCoord * 2.0 - iResolution.xy) / iResolution.y + vec2(-0.0, -0.0);

    vec3 col = vec3(0);
    
    uv *= GS;
    
    vec2 r = vec2(1,1.73);
    vec2 h = r*.5;
    vec2 gv1 = mod(uv, r)-h;
    vec2 gv2 = mod(uv+h, r)-h;
    vec2 gv = dot(gv1,gv1) < dot(gv2,gv2) ? gv1 : gv2;
    vec2 id = uv-gv;
    gv *= 2.;
    
    //col.rg += id*.1;
    //col.rg += gv;
    
    float oe = round(R1(id*.01 + .01*vec2(.3112,.7)*iTime));
    float oa = PI/3.*oe;
    float a = mod(atan(gv.y, gv.x) -oa, 2.*PI);
    float sc = mod(floor((a +PI*0.1666666) / (PI/1.5)), 3.);
    float ca = sc*PI/1.5 +PI*0.1666666 +oa;
    vec2 bv = gv - vec2(cos(ca), sin(ca))*1.15;
    
    float cd = distance(bv, vec2(0));
    float width = .3;
    float mask = max(0., 
              smoothstep(.577+width+SMH, .577+width,cd) 
            - smoothstep(.577-width, .577-width-SMH,cd));
    

    vec2 D = normalize(bv);
    vec3 n = normalize(vec3((cd-.577)/width*2. * D, 1.));

    col += mask * n;
    col += (1.-mask)*vec3(0,0,1);
    col = normalize(col);
    
    //specular
    col += mask * max(.0001,pow(dot(reflect(-normalize(vec3(.5,.8,.8)),n), vec3(0,0,1)),64.));

    
    fragColor = vec4(col,1.0);
}