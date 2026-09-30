#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "Buffers.h"

enum ShaderType{
    OBJECT = 0,
    LIGHT = 1
};

/*
	Shaders create and compile vertex and fragment shaders, and link them into a shader program.
	shader obj handles the shader program, and provides an interface to bind and unbind the shader.
*/
class Shader 
{
public:
    Shader() = default;
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    Shader(const Shader&) = delete; //no copy
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept; // Move 
    Shader& operator=(Shader&& other) noexcept;

    void use();
    void bind(bool t);

    unsigned int* getId() { return &m_RenderID; }

    void setMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);

    void setUniform1Int(const char* varName, const GLint& v0 = 0);
    void setUniformMat4f(const char* varName, const glm::mat4& value);
    void setUniformVec3(const char* varName, const glm::vec3 value);
    void setUniform1Float(const char* varName, const float value);

    const int getUniformLoc(const char* uName);

private:
	unsigned int m_RenderID;

    std::unordered_map<const char*, int> m_UniformLocationCache;

    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};

