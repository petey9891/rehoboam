#include <Renderer.h>
#include <Shader.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

Shader::Shader(const std::string& folderPath): m_RendererID(0), m_FolderPath(folderPath) {
    printf(">>> <Shader> Initializing shader from file %s\n", this->m_FolderPath.c_str());

    // ShaderProgramSource source = this->parseShaders(this->m_FolderPath);

    this->m_RendererID = this->createShaders();

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

std::string Shader::parseShader(const std::filesystem::path path) {
    std::ifstream ifs(path);
    if(!ifs)
        throw(std::runtime_error("File not opened."));
    std::ostringstream stream;
    stream<<ifs.rdbuf();
    return stream.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::vector<fs::path> sourceFiles) {
    std::string shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
    std::vector<std::string> sources;
    for (fs::path path : sourceFiles) {
        printf(">>>>>> <Shader> Parsing %s\n", path.c_str());
        std::string src = this->parseShader(path);
        assert(src.size() <= this->MAX_SIZE);
        sources.push_back(src);
    }

    std::vector<const char*> data;
    for (std::string s : sources) {
        data.push_back(s.c_str());
    }

    printf(">>>>>> <Shader> Generating %s source files\n", shaderType.c_str());
    GLCall(unsigned int id = glCreateShader(type));

    // const char* data[] = { sources[0].c_str() };
    
    GLCall(glShaderSource(id, sourceFiles.size(), data.data(), nullptr));

    printf(">>>>>> <Shader> Compiling %s shader\n", shaderType.c_str());
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

std::vector<fs::path> Shader::aggregateShaders(const ShaderType type) {
    std::vector<fs::path> files;

    std::string path = this->m_FolderPath;
    if (type == VERTEX) {
        path += "/vertex";
    } else {
        path += "/fragment"; 
    }

    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        files.push_back(entry.path());
    }

    return files;
}

unsigned int Shader::createShaders() {
    // create a shader program
    unsigned int program = glCreateProgram();

    std::vector<fs::path> vertexFiles = this->aggregateShaders(VERTEX);
    std::vector<fs::path> fragmentFiles = this->aggregateShaders(FRAGMENT);

    unsigned int vs = this->compileShader(GL_VERTEX_SHADER, vertexFiles);
    unsigned int fs = this->compileShader(GL_FRAGMENT_SHADER, fragmentFiles);

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
