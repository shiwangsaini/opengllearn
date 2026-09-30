#include "shaders.h"

#include <malloc.h>
#include <fstream>


Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    m_RenderID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(m_RenderID);
}

Shader::Shader(Shader&& other) noexcept
    : m_RenderID(other.m_RenderID)
{
    other.m_RenderID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (m_RenderID != 0)
            glDeleteProgram(m_RenderID);

        m_RenderID = other.m_RenderID;
        other.m_RenderID = 0;
    }

    return *this;
}

void Shader::use()
{
    glUseProgram(m_RenderID);
}

void Shader::bind(bool t)
{
    this->use();
}

void Shader::setMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
    setUniformMat4f("uModel", M);
    setUniformMat4f("uView", V);
    setUniformMat4f("uProj", P);
}

void Shader::setUniform1Int(const char* varName, const GLint& v0)
{
    glUniform1i(getUniformLoc(varName), v0);
}

void Shader::setUniformMat4f(const char* varName, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniformLoc(varName), 1, GL_FALSE, &(value[0][0]));
}

void Shader::setUniformVec3(const char* varName, const glm::vec3 value)
{
    glUniform3f(getUniformLoc(varName), value.x, value.y, value.z);

}

void Shader::setUniform1Float(const char* varName, const float value)
{
    glUniform1f(getUniformLoc(varName), value);
}


const int Shader::getUniformLoc(const char* uName)
{
    if (m_UniformLocationCache.find(uName) == m_UniformLocationCache.end())
        return m_UniformLocationCache[uName];

    int loc = glGetUniformLocation(m_RenderID, uName);
    if (loc == -1) {
        std::cerr << "Lol Uniform location Not found : " << uName << " <- This\n";
    }
    else m_UniformLocationCache[uName] = loc;

    return loc;
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
        char* message = (char*)_malloca(length * sizeof(char));

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