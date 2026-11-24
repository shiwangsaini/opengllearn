#include "shaders.h"

#include <malloc.h>
#include <fstream>

Shader::~Shader()
{
    glUseProgram(0);
    for(auto& id : mRenderIdCache)
        glDeleteProgram(id.RenderID);
    mRenderIdCache.clear();
}

Shader::Shader(Shader&& other) noexcept
    : mRenderIdCache(other.mRenderIdCache)
{
    for(auto& oid: other.mRenderIdCache)
        oid.RenderID = 0;
    other.mRenderIdCache.clear();
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        for (auto& id : mRenderIdCache) {
            if (id.RenderID!= 0)
                glDeleteProgram(id.RenderID);
        }
        mRenderIdCache = other.mRenderIdCache;
        for(auto& oid: other.mRenderIdCache)
            oid.RenderID = 0;
    }

    return *this;
}

void const Shader::use(const uint16_t& index) const
{
    if (index >= mRenderIdCache.size())
        throw "LOL Not have the shader of this Index";
    glUseProgram(mRenderIdCache[index].RenderID);
}

bool const Shader::AddShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int sh = CreateShader(vertexShader, fragmentShader);
    if (sh < 0) return -1;
    mRenderIdCache.push_back(sh);
    return true;
}

void Shader::setMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P, const uint16_t& shaderType)
{
    setUniformMat4f("uModel", M, shaderType);
    setUniformMat4f("uView" , V, shaderType);
    setUniformMat4f("uProj" , P, shaderType);
}

void Shader::setUniform1Int(const char* varName, const GLint& v0, const uint16_t& index)
{
    glUniform1i(getUniformLoc(varName, index), v0);
}

void Shader::setUniformMat4f(const char* varName, const glm::mat4& value, const uint16_t& index)
{
    glUniformMatrix4fv(getUniformLoc(varName, index), 1, GL_FALSE, &(value[0][0]));
}

void Shader::setUniformVec3(const char* varName, const glm::vec3 value, const uint16_t& index)
{
    glUniform3f(getUniformLoc(varName, index), value.x, value.y, value.z);

}

void Shader::setUniform1Float(const char* varName, const float value, const uint16_t& index)
{
    glUniform1f(getUniformLoc(varName, index), value);
}


const int Shader::getUniformLoc(const char* uName, const uint16_t& index)
{
    // Return cached location if it exists.
    if (mRenderIdCache[index].UniformLocationCache.find(uName) != mRenderIdCache[index].UniformLocationCache.end())
        return mRenderIdCache[index].UniformLocationCache[uName];
    // Otherwise, query OpenGL.
    GLint loc = glGetUniformLocation(mRenderIdCache[index].RenderID, uName);
    if (loc == -1)
    {
        std::cerr << "Lol Uniform location Not found : " << uName << " <- This\n";
    }
    mRenderIdCache[index].UniformLocationCache[uName] = loc;
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