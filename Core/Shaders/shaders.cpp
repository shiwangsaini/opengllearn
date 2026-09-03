#include "shaders.h"

#include <malloc.h>
#include <fstream>


Shader::Shader()
{
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(m_RenderID);
}

void Shader::init(const std::string& vertexShader, const std::string& fragmentShader)
{
    m_RenderID = CreateShader(vertexShader, fragmentShader);
}

void Shader::use()
{
    glUseProgram(m_RenderID);
}

void Shader::bind()
{ 
    this->use();
    VB.bind();
    IB.bind();
}

void Shader::setUniform1i(const char* varName, const GLint& v0)
{
    glUniform1i((glGetUniformLocation(m_RenderID, varName)), v0);
}

void Shader::setUniformMat4f(const char* varName, const glm::mat4& value)
{
    glUniformMatrix4fv((glGetUniformLocation(m_RenderID, varName)), 1, GL_FALSE, &(value[0][0]));
}

/*
    compile shader takes in the type of shader (vertex or fragment) and the source code, and compiles the shader.
*/
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    std::ifstream shader_file(source);
    std::string shader_read;

    std::getline(shader_file, shader_read, '\0');

    unsigned int id = glCreateShader(type);
    const char* src = shader_read.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check for compilation errors
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length*sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile Shader : " << (type == GL_VERTEX_SHADER ? "Vertex : " : "Fragment : ") << message << std::endl;
        glDeleteShader(id);
    }

    return id;
}

/*
    create shader takes in the vertex and fragment shader source code, compiles them, and links them into a shader program.
*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);    // attach the vertex shader to the program
    glAttachShader(program, fs);    // attach the fragment shader to the program
    glLinkProgram(program);         // link the program
    glValidateProgram(program);     // validate the program

    // delete the shaders after linking them into the program, as they are no longer needed
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}