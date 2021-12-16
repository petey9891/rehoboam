#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

class Shader {
    public: 
        GLuint id;
    private:
        std::unordered_map<std::string, int> m_UniformLocationCache;
        std::unordered_map<std::string, int> m_AttributeLocationCache;
        enum ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

    public:
        Shader(const char* vertexFile, const char* fragmentFile);
        ~Shader();

        void bind() const;
        void unbind() const;

        void setUniform1f(const std::string& name, float value);
        void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
        void setUniform1fv(const std::string& name, float count, const float* value);

        GLint getUniform(const std::string& name);
        GLint getAttribute(const std::string& name);

    private:
        // Shader getters
        int getUniformLocation(const std::string& name);
        int getAttributeLocation(const std::string& name);
        
        // Shader generation
        unsigned int createShaders(const char* vertexFile, const char* fragmentFile);
        void compileErrors(unsigned int shader, const char* type);
};