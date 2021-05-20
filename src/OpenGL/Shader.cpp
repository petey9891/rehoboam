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

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
    printf(">>>>>> <Shader> Compiling %s shader\n", shaderType.c_str());
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();

    std::string path = "/home/pi/rehoboam/shaders/rehoboam/" + shaderType + ".shader";
    std::ifstream iStream(path);
    std::stringstream buffer;
    buffer << iStream.rdbuf();
    std::string dataSrc = buffer.str();

    const char* sources[] = { dataSrc.c_str() };
    printf("%d\n", source.size());
    printf("%d\n", strlen(dataSrc.c_str()));

    GLint length[] = { strlen(dataSrc.c_str() )}
    GLCall(glShaderSource(id, 1, sources, length));
    GLCall(glCompileShader(id));

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
    unsigned int vs = this->compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = this->compileShader(GL_FRAGMENT_SHADER, fragmentShader);

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