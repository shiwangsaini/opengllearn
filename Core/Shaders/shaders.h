#pragma once

#include <string>
#include <iostream>
#include <glad/gl.h>

#include <glm.hpp>
/*
	Shaders create and compile vertex and fragment shaders, and link them into a shader program.
	shader obj handles the shader program, and provides an interface to bind and unbind the shader.
*/
class Shader 
{
public:
    Shader() {}
    ~Shader();

	void init(const std::string& vertexShader, const std::string& fragmentShader);
    void bind();
    void unbind();

    void setUniform1i(const char* varName, const GLint& v0 = 0);
    void setUniformMat4f(const char* varName, const glm::mat4& value);

	unsigned int m_RenderID;

private:
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};

