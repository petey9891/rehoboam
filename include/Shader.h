#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    const char* vertexSources[];
    const char* fragmentSources[];
};

class Shader {
    public: 
        unsigned int m_RendererID;
    private:
        std::string m_FolderPath;
        std::unordered_map<std::string, int> m_UniformLocationCache;
        std::unordered_map<std::string, int> m_AttributeLocationCache;
        enum ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

    public:
        Shader(const std::string& filepath);
        ~Shader();

        void bind() const;
        void unbind() const;

        void setUniform1f(const std::string& name, float value);
        void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
        void setUniform1fv(const std::string& name, float count, const float* value);

        GLint getUniform(const std::string& name);
        GLint getAttribute(const std::string& name);

    private:
        int getUniformLocation(const std::string& name);
        int getAttributeLocation(const std::string& name);
        struct ShaderProgramSource parseShaders(const std::string& path);
        unsigned int compileShader(unsigned int type, const std::string& source);
        unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

};