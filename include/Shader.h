#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

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
        unsigned int MAX_SIZE = 2032;

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
        // Shader getters
        int getUniformLocation(const std::string& name);
        int getAttributeLocation(const std::string& name);
        
        // Shader generation
        const char* convert(const std::string& s);
        std::string parseShader(const std::filesystem::path path);
        std::vector<std::filesystem::path> aggregateShaders(const ShaderType type);
        unsigned int compileShader(unsigned int type, const std::vector<std::filesystem::path> sourceFiles);
        unsigned int createShaders();

};