#include <Renderer.h>
#include <Shader.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    printf(">>> <Shader> Initializing shader\n");
    this->createShaders(vertexFile, fragmentFile);
    GLCall(glUseProgram(this->id));
    printf(">>> <Shader> Completed initializing shader\n");
}

Shader::~Shader() {
    GLCall(glDeleteProgram(this->id));
}

void Shader::bind() const {
    GLCall(glUseProgram(this->id));
}

void Shader::unbind() const {
    GLCall(glDeleteProgram(this->id));
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

    GLCall(int location = glGetUniformLocation(this->id, name.c_str()));
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

    GLCall(int location = glGetAttribLocation(this->id, name.c_str()));
    if (location == -1) {
        printf("No active attribute variable with name %s found\n", name.c_str());
    }

    this->m_AttributeLocationCache[name] = location;

    return location;
}

unsigned int Shader::createShaders(const char* vertexFile, const char* fragmentFile) {
    // Read vertexFile and fragmentFile and store the strings
    printf(">>> <Shader> Parsing vertex shader\n");
	std::string vertexCode = get_file_contents(vertexFile);
    printf(">>> <Shader> Parsing fragment shader\n");
	std::string fragmentCode = get_file_contents(fragmentFile);
    
    // Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
    
    printf(">>> <Shader> Compiling vertex shader\n");
	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

    printf(">>> <Shader> Compiling fragment shader\n");
	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(vertexShader, "FRAGMENT");

    printf(">>> <Shader> Creating program\n");
	// Create Shader Program Object and get its reference
	this->id = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(this->id);
	compileErrors(vertexShader, "PROGRAM");

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "SHADER_COMPLETION_ERROR for:" << type << "\n" << std::endl;
		}
	} else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}
