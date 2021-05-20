#include <Renderer.h>
#include <Shader.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath): m_RendererID(0), m_FilePath(filepath) {
    printf(">>> <Shader> Initializing shader from file %s\n", this->m_FilePath.c_str());

    // printf("filepath: %s\n", filepath.c_str());
    ShaderProgramSource source = this->parseShader(filepath);

    // printf("\nVertex Shader:\n%s\n", source.vertexSource.c_str());
    // printf("\nFragment Shader:\n%s\n\n", source.fragmentSource.c_str());

    this->m_RendererID = this->createShader(source.vertexSource, source.fragmentSource);

    GLCall(glUseProgram(this->m_RendererID));

    printf(">>> <Shader> Initialized shader with id: %d\n", this->m_RendererID);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(this->m_RendererID));
}

void Shader::bind() const {
    GLCall(glUseProgram(this->m_RendererID));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(this->getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
    GLCall(glUniform4f(this->getUniformLocation(name), v1, v2, v3, v4));
}

void Shader::setUniform1fv(const std::string& name, float count, const float* value) {
    GLCall(glUniform1fv(this->getUniformLocation(name), count, value));
}

GLint Shader::getUniform(const std::string& name) {
    return this->getUniformLocation(name);
}

GLint Shader::getAttribute(const std::string& name) {
    return this->getAttributeLocation(name);
}

int Shader::getUniformLocation(const std::string& name) {
    if (this->m_UniformLocationCache.find(name) != this->m_UniformLocationCache.end())
        return this->m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(this->m_RendererID, name.c_str()));
    if (location == -1) {
        printf("No active uniform variable with name %s found\n", name.c_str());
    }

    this->m_UniformLocationCache[name] = location;

    return location;
}

int Shader::getAttributeLocation(const std::string& name) {
    if (this->m_AttributeLocationCache.find(name) != this->m_AttributeLocationCache.end()) {
        return this->m_AttributeLocationCache[name];
    }

    GLCall(int location = glGetAttribLocation(this->m_RendererID, name.c_str()));
    if (location == -1) {
        printf("No active attribute variable with name %s found\n", name.c_str());
    }

    this->m_AttributeLocationCache[name] = location;

    return location;
}

struct ShaderProgramSource Shader::parseShader(const std::string& filepath) {

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = NONE;

    if (stream.fail()) {
        printf("****** Unable to find shader at %s\n", filepath.c_str());
    }

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    struct ShaderProgramSource sps = { ss[0].str(), ss[1].str() };
    return sps;
}

const char* const vert = 1 + R"GLSL(
attribute vec3 pos;
attribute vec2 coord;

varying vec2 backgroundCoord;

void main() {
    backgroundCoord = coord;
    gl_Position = vec4(pos, 1.0);
}
)GLSL";

const char* const circle = 1 + R"GLSL(
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
)GLSL";
 
const char* const frag = 1 + R"GLSL(
#version 100
const int CORES = 8;
const float T1 = 40.0;
const float T2 = 60.0;
const float T3 = 80.0;
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

uniform float time;
uniform float age;
uniform float fade;

varying vec2 backgroundCoord;

float phi;
float threadf = 0.0;

void main() {
    vec2 coords = backgroundCoord.xy*0.5;
    float radius = 0.25;

    float phi = (atan(coords.y, coords.x)+3.1415926538)/3.1415926538*float(CORES)*0.5;    
    vec2 i = background;    
    float c = 1.0;    

    for (int n = 0; n < 8; n++) {
        float t = time * (0.7 - (0.2 / float(n+1)));        
        c += 1.0 / length(vec2(background.x / (2.0 * sin(i.x + t) / inten), background.y / (cos(i.y + t) / inten)));
    }

    c /= 8.0;
    c = 1.5 - sqrt(pow(c, 2.0));
        
    bColor = smoothstep(T2, T1, temperature)*bColor + smoothstep(T1, T2, temperature)*smoothstep(T3, T2, temperature)*bColorWarm + smoothstep(T2, T3, temperature)*bColorHot;
    
    rColor = smoothstep(50.0, 0.0, threadf)*rColor + smoothstep(0.0, 50.0, threadf)*smoothstep(100.0, 50.0, threadf)*rColorWarm + smoothstep(50.0, 100.0, threadf)*rColorHot;
    // rColor *= circle(coords, radius, 0.01);
        
    vec3 outcolor = bColor * c * c * c * c + rColor;

    float coreIndex = 0.0;
    for (int i = 0; i < CORES; i++) {
        threadf += clamp(1.0-abs(phi-coreIndex), 0.0, 1.0)*thread[i];
        coreIndex += 1.0;
    }

    outcolor *= vec3(fade);

    gl_FragColor = vec4(mix(outcolor, outcolor, smoothstep(5.0, 10.0, age)), 1.0);
}
)GLSL";

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
    printf(">>>>>> <Shader> Compiling %s shader\n", shaderType.c_str());
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();

    // std::string path = "/home/pi/rehoboam/shaders/rehoboam/" + shaderType + ".shader";
    // std::ifstream iStream(path);
    // std::stringstream buffer;
    // buffer << iStream.rdbuf();
    // std::string dataSrc = buffer.str();

    // if (type == GL_FRAGMENT_SHADER) {
    //     const char* sources[] = { source.c_str(), circle };

    //     printf("%d\n", source.size());
    //     printf("%d\n", strlen(source.c_str()));
    //     printf("%d\n", strlen(circle));

    //     GLint length[] = { strlen(source.c_str()), strlen(circle) };
    //     GLCall(glShaderSource(id, 2, sources, length));
    //     GLCall(glCompileShader(id));
    // } else {
        const char* sources[] = { source.c_str() };

        printf("%d\n", source.size());
        printf("%d\n", strlen(source.c_str()));

        GLint length[] = { strlen(source.c_str()) };
        GLCall(glShaderSource(id, 1, sources, length));
        GLCall(glCompileShader(id));
    // }
    

    // Current max length 2032
    // Current working length 2032 -- no variance or any circle code


    // WORKING:
    // printf("%d\n", source.size());
    // printf("%d\n", strlen(src));

    // GLint length[] = { strlen(src) };
    // GLCall(glShaderSource(id, 1, &src, length));
    // GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        printf("****** Failed to compile %s shader\n", shaderType.c_str());
        printf("%s\n", message);

        GLCall(glDeleteShader(id));
        return 0;
    }
    printf(">>>>>> <Shader> %s shader compiled\n", shaderType.c_str());
    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = this->compileShader(GL_VERTEX_SHADER, vert);
    unsigned int fs = this->compileShader(GL_FRAGMENT_SHADER, frag);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));

    GLint program_linked;

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &program_linked));
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall(glGetProgramInfoLog(program, 1024, &log_length, message));
        std::cout << "****** Failed to link program" << std::endl;
        std::cout << message << std::endl;
    } else {
        printf(">>>>>> <Shader> Successfully linked program\n");
    }

    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
