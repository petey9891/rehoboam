#include "led-matrix.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <math.h>
#include <random>
#include <chrono>
#include <unordered_map>

//Animation speed
#define ANIMSTEP 0.5

//Resolution, three panels with 64x64 each.
#define W 192
#define H 64

//Number of thread cores (or threads) to be represented on the ring
// #define CORES 256
#define CORES 128

//Temperature thresholds for cool (blue), medium (green/yellow) and hot (red)
#define T1 40.0 //Do not omit the decimal point. This will be used in the OpenGL shader which will otherwise interpret it as a float literal
#define T2 60.0
#define T3 80.0

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace rgb_matrix;
using namespace std;

unordered_map<int, float> discrepancies;

float temperature = 30.0f;
float thread[CORES];
float tmpTemperature = 40.0f;
float tmpThread[CORES];

float t = 0.0f;
float updateTime = -10.0f;

volatile bool interrupt_received = false;

static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8,

    EGL_SAMPLE_BUFFERS, 1,
    EGL_SAMPLES, 4,

    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE};

// Width and height of the desired framebuffer
static const EGLint pbufferAttribs[] = {
    EGL_WIDTH,
    W,
    EGL_HEIGHT,
    H,
    EGL_NONE,
};

static const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                        EGL_NONE};


static const GLfloat vertices[] = {
    -1.0f,
    -1.0f,
    0.0f,
    -1.0f,
    1.0f,
    0.0f,

    -0.33333333333f,
    -1.0f,
    0.0f,
    -0.33333333333f,
    1.0f,
    0.0f,

    -0.33333333333f,
    -1.0f,
    0.0f,
    -0.33333333333f,
    1.0f,
    0.0f,

    0.33333333333f,
    -1.0f,
    0.0f,
    0.33333333333f,
    1.0f,
    0.0f,

    0.33333333333f,
    -1.0f,
    0.0f,
    0.33333333333f,
    1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
};

static const GLfloat vcoords[] = {
    0.0f,
    0.0f,
    -0.866f,
    0.5f,

    0.0f,
    -1.0f,
    -0.866f,
    -0.5f,

    0.0f,
    0.0f,
    0.0f,
    -1.0f,

    0.866f,
    0.5f,
    0.866f,
    -0.5f,

    0.0f,
    0.0f,
    0.866f,
    0.5f,

    -0.866f,
    0.5f,
    0.0f,
    1.0f,
};

// Shaders
#define STRINGIFY(x) #x
#define RESTRINGIFY(x) STRINGIFY(x)
static const char* vertexShaderCode = STRINGIFY(
    attribute vec3 pos;
    attribute vec2 coord;
    varying vec2 backgroundCoord;
    void main() {
        backgroundCoord = coord;
        gl_Position = vec4(pos, 1.0);
    }
);

static const char* backgroundShaderMain = STRINGIFY(
    const int CORES = ) RESTRINGIFY(CORES) STRINGIFY(;
    const float T1 = ) RESTRINGIFY(T1) STRINGIFY(;
    const float T2 = ) RESTRINGIFY(T2) STRINGIFY(;
    const float T3 = ) RESTRINGIFY(T3) STRINGIFY(;
    uniform float temperature;
    uniform float thread[CORES];

	// background colors
    vec3 bColor = vec3(0.1, 0.6, 0.4);
    vec3 bColorWarm = vec3(0.5, 0.5, 0.1);
    vec3 bColorHot = vec3(0.6, 0.2, 0.1);

	// ring colors
    vec3 rColor = vec3(0.7, 1.0, 0.9);
    vec3 rColorWarm = vec3(1.0, 1.0, 0.6);
    vec3 rColorHot = vec3(1.0, 1.0, 1.0);
    
	uniform float age;
    uniform float time;
    varying vec2 backgroundCoord;

    float phi;
    float threadf = 0.0;

    float circle(vec2 uv, float w0, float width) {
        float f = length(uv) + (sin(normalize(uv).y * 5.0 + time * 2.0) - sin(normalize(uv).x * 5.0 + time * 2.0)) / 100.0;
        float w = width + width*threadf*0.1;
        return smoothstep(w0-w, w0, f) - smoothstep(w0, w0+w, f);
    }

    void main() {
        vec2 coords = backgroundCoord.xy*0.5;
        float phi = (atan(coords.y, coords.x)+3.1415926538)/3.1415926538*float(CORES)*0.5;
);

// I assume this makes a section of the loop larger depending on the load the core has
void threadShader(char *str, int i) {
    sprintf(str, "threadf += clamp(1.0-abs(phi-%d.0), 0.0, 1.0)*thread[%d];", i, i % CORES);
}

static const char* backgroundColorShader = STRINGIFY(
        vec2 background = backgroundCoord.xy * 0.5 * 10.0 - vec2(19.0); // background
        vec2 i = background;
        float c = 1.0; // level of brightness for background?
        float inten = 0.05; // background alpha/brightness?
        
        for (int n = 0; n < 8; n++) {
        	float t = time * (0.7 - (0.2 / float(n+1)));
            	i = background + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x)); // initial smoothing of waves
            	c += 1.0 / length(vec2(background.x / (2.0 * sin(i.x + t) / inten), background.y / (cos(i.y + t) / inten)));
        }
        
        c /= 8.0;
        c = 1.5 - sqrt(pow(c, 2.0));

		// background
        bColor.g = clamp(coords.x, 0.0, 1.0); // adds the gradient
        bColor = smoothstep(T2, T1, temperature)*bColor + smoothstep(T1, T2, temperature)*smoothstep(T3, T2, temperature)*bColorWarm + smoothstep(T2, T3, temperature)*bColorHot;
         
		// ring
        rColor = smoothstep(50.0, 0.0, threadf)*rColor + smoothstep(0.0, 50.0, threadf)*smoothstep(100.0, 50.0, threadf)*rColorWarm + smoothstep(50.0, 100.0, threadf)*rColorHot;
		rColor *= circle(coords, 0.25, 0.01);
	
		// color output
		// multiplying c increases wave intensity
		vec3 outcolor = bColor * c * c * c * c + rColor;

    	gl_FragColor = vec4(mix(outcolor, outcolor, smoothstep(5.0, 10.0, age)), 1.0);
    }
);

bool checkShader(GLuint shader) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
	    GLint length = 0;
	    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	    std::vector<GLchar> errorLog(length);
	    glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
        fprintf(stderr, "Shader compilation failed!\n%s\n", errorLog);
        return false;
    }
    return true;
}

void addDisorder(int index, float* threads, bool multiplier) {
	float multi = multiplier ? CORES * 0.1f : 1.0f;
	threads[index] += ANIMSTEP * multi;

	if (thread[index] >= 60 && discrepancies.find(index) == discrepancies.end()) {
		printf("<addDisorder> Adding discrepancy %d, current value: %f\n", index, threads[index]);
		discrepancies[index] = threads[index];
		printf("number of discrepancies: %d/%d\n", discrepancies.size(), CORES);
	} 
}

void removeDisorder(int index, float* threads, bool multiplier) {
	float multi = multiplier ? CORES * 0.1f : 1.0f;
	if (threads[index] <= ANIMSTEP * multi) {
		thread[index] = 0.0f;
	} else if (threads[index] >= ANIMSTEP * multi) {
		threads[index] -= ANIMSTEP * multi;
	} else if (threads[index] >= ANIMSTEP) {
		threads[index] -= ANIMSTEP;
	} else {
		threads[index] = 0.0f;
	}

	if (threads[index] > 200.0f) {
		threads[index] -= 50.0f;
	}

	if (threads[index] <= 60 && discrepancies.find(index) != discrepancies.end()) {
		printf("<removeDisorder> Removing discrepancy %d, current value: %f\n", index, threads[index]);
		discrepancies.erase(index);
		printf("number of discrepancies: %d/%d\n", discrepancies.size(), CORES);
	}
}

int main(int argc, char* argv[]) {
	EGLDisplay display;
	int major;
	int minor;
	int desiredWidth;
	int desiredHeight;

	GLuint program, vert, frag, vbo, vbocoord;
	GLint posLoc, coordLoc, temperatureLoc, threadLoc, timeLoc, ageLoc, result;

	// Setup EGL
	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		fprintf(stderr, "Failed to get EGL display! Error: %s\n", eglGetError());
		return EXIT_FAILURE;
	}

	if (eglInitialize(display, &major, &minor) == EGL_FALSE) {
		fprintf(stderr, "Failed to get EGL version! Error: %s\n", eglGetError());
		eglTerminate(display);
		return EXIT_FAILURE;
	}

	printf("Initialized EGL version: %d.%d\n", major, minor);

	// EGL Config
	EGLint numConfigs;
	EGLConfig config;
	if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs)) {
		fprintf(stderr, "Failed to get EGL Config! Error: %s\n", eglGetError());
		eglTerminate(display);
		return EXIT_FAILURE;
	}

	// EGL Surface
	EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
	if (surface == EGL_NO_SURFACE) {
		fprintf(stderr, "Failed to create EGL surface! Error: %s\n", eglGetError());
		eglTerminate(display);
		return EXIT_FAILURE;
	}

	// Bind OpenGL API
	eglBindAPI(EGL_OPENGL_API);

	// EGL Context
	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		fprintf(stderr, "Failed to create EGL context! Error: %s\n", eglGetError());
		eglDestroySurface(display, surface);
		eglTerminate(display);
		return EXIT_FAILURE;
	}

	eglMakeCurrent(display, surface, surface, context);

	desiredWidth = pbufferAttribs[1];
	desiredHeight = pbufferAttribs[3];

	glViewport(0, 0, desiredWidth, desiredHeight);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	printf("GL Viewport size: %dx%d\n", viewport[2], viewport[3]);


	if (desiredWidth != viewport[2] || desiredHeight != viewport[3]) {
		fprintf(stderr, "Error! the glViewport/glgetIntergerv are not working! EGL might be faulty!\n");
		return EXIT_FAILURE;
	}
	
	// Clear the whole screen (front buffer)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Shader program
	program = glCreateProgram();
	glUseProgram(program);
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertexShaderCode, nullptr);
	glCompileShader(vert);
	
	if (!checkShader(vert)) {
		return EXIT_FAILURE;
	}

	// Create buffer for shader code
	const int codeLength = strlen(backgroundShaderMain) + (CORES+1) * 60 + strlen(backgroundColorShader);
	char backgroundShader[codeLength];

	// Add main shader to buffer
	strcpy(backgroundShader, backgroundShaderMain);

	// Add individual thread shader to buffer
	for (int i = 0; i <= CORES; i++) {
		char loopCode[60];
		threadShader(loopCode, i);
		strcat(backgroundShader, loopCode);
	}

	// Add color shader to buffer
	strcat(backgroundShader, backgroundColorShader);

	static const char* compiledBackgroundShader = backgroundShader;

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &compiledBackgroundShader, nullptr);
	glCompileShader(frag);
	if (!checkShader(frag)) {
		return EXIT_FAILURE;
	}

	// Attach shaders
	glAttachShader(program, frag);
	glAttachShader(program, vert);
	glLinkProgram(program);
	glUseProgram(program);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbocoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbocoord);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vcoords, GL_STATIC_DRAW);

	// Get vertex attribute and uniform locations
	posLoc = glGetAttribLocation(program, "pos");
	coordLoc = glGetAttribLocation(program, "coord");
	temperatureLoc = glGetUniformLocation(program, "temperature");
	threadLoc = glGetUniformLocation(program, "thread");
	timeLoc = glGetUniformLocation(program, "time");
	ageLoc = glGetUniformLocation(program, "age");

	// Set our vertex data
	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

	glEnableVertexAttribArray(coordLoc);
	glBindBuffer(GL_ARRAY_BUFFER, vbocoord);
	glVertexAttribPointer(coordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);


	// LED Matrix Settings
	RGBMatrix::Options defaults;
	rgb_matrix::RuntimeOptions runtime;

	defaults.hardware_mapping = "adafruit-hat-pwm";
	defaults.led_rgb_sequence = "BGR";
	defaults.pwm_bits = 11;
	defaults.pwm_lsb_nanoseconds = 50;
	// defaults.panel_type = "FM6126A";
	defaults.rows = 64;
	defaults.cols = 192;
	defaults.chain_length = 1;
	defaults.parallel = 1;

	runtime.drop_privileges = 0;
	runtime.gpio_slowdown = 1;

	// Create matrix
	RGBMatrix* matrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults, &runtime);

	if (matrix == nullptr) {
		fprintf(stderr, "Error! Unable to create matrix!\n");
		return EXIT_FAILURE;
	}

	// Create canvas
	FrameCanvas* canvas = matrix->CreateFrameCanvas();

	// Buffer of pixels
	unsigned char* buffer = (unsigned char*)malloc(W * H * 3);

	// Setup randomization
	std::random_device rd;
	// seed value is designed specifically to make initialization
    // parameters of std::mt19937 (instance of std::mersenne_twister_engine<>)
    // different across executions of application
	// https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen(seed);
	for (int i = 0; i < CORES; i++) {
		thread[i] = 0.0f;
	}  

	int maxDiscrepancies = CORES * 0.8f;
	while (!interrupt_received) {
		t += 0.01f;

		bool multiplier = (gen() % 100 + 1) == 57;
		if (discrepancies.size() < maxDiscrepancies) {
			// try to balance it out but not adding anymore disorder
			int addThreadIndex = gen() % CORES;
			addDisorder(addThreadIndex, thread, multiplier);
		}

		int removeThreadIndex = gen() % CORES;
		removeDisorder(removeThreadIndex, thread, multiplier);

		glUniform1f(timeLoc, t);
		glUniform1f(ageLoc, float(t - updateTime));


		if (discrepancies.size() >= CORES/2 && temperature < 95.0f) { // 6
			// cube will be red
			temperature += 0.02f*ANIMSTEP;
		} else if (discrepancies.size() >= CORES/3) { // 4
			// cube will be yellow/orange
			if (temperature > 69) {
				temperature -= 0.05f*ANIMSTEP;
			} else if (temperature < 69) {
				temperature += 0.02f*ANIMSTEP;
			}
		} else if (discrepancies.size() >= CORES/4) { // 3
			if (temperature > 52) {
				temperature -= 0.05f*ANIMSTEP;
			} else if (temperature <= 52) {
				temperature += 0.02f*ANIMSTEP;
			}
		} else if (temperature > 30) {
			// cube will start to lose color be blue/yellow
			temperature -= 0.05f*ANIMSTEP;
		}


		glUniform1f(temperatureLoc, temperature);
		glUniform1fv(threadLoc, CORES, thread);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

		glReadPixels(0, 0, W, H, GL_RGB, GL_UNSIGNED_BYTE, buffer);

		for (int x = 0; x < W; x++) {
			for (int y = 0; y < H; y++) {
				int index = 3*(x+y*W);
				canvas->SetPixel(x, y, buffer[index], buffer[index+1], buffer[index+2]);
			}
		}
		canvas = matrix->SwapOnVSync(canvas);

	}

	// Cleanup
	free(buffer);
	canvas->Clear();

	eglDestroyContext(display, context);
	eglDestroySurface(display, surface);
	eglTerminate(display);

	return EXIT_SUCCESS;
}

