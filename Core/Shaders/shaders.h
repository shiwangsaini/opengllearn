#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "Buffers.h"

enum ShaderType : uint16_t{
    OBJECT = 0,
    LIGHT = 1,
    ENEMY = 2
};

struct ShaderIdUniformCache {
    ShaderIdUniformCache(const unsigned int& id) :RenderID(id) {};
    unsigned int RenderID;
    std::unordered_map<const char*, GLint> UniformLocationCache;
};

/*
	Shaders create and compile vertex and fragment shaders, and link them into a shader program.
	shader obj handles the shader program, and provides an interface to bind and unbind the shader.
*/
class Shader 
{
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&)               = delete;
    Shader& operator=(const Shader&)    = delete;
    Shader(Shader&& other)              noexcept;
    Shader& operator=(Shader&& other)   noexcept;

    void const use          (const uint16_t& index = 0) const;
    bool const AddShader    (const std::string& vertexShader, const std::string& fragmentShader);

    const unsigned int& getId   (const uint16_t& index = 0) { return mRenderIdCache[index].RenderID; }

    void setMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P, const uint16_t& shaderType);

    void setUniform1Int     (const char* varName, const GLint& v0 = 0, const uint16_t& index = 0);
    void setUniformMat4f    (const char* varName, const glm::mat4& value, const uint16_t& index = 0);
    void setUniformVec3     (const char* varName, const glm::vec3 value, const uint16_t& index = 0);
    void setUniform1Float   (const char* varName, const float value, const uint16_t& index = 0);

    const int getUniformLoc (const char* uName, const uint16_t& index = 0);

private:
    std::vector<ShaderIdUniformCache> mRenderIdCache;

    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

