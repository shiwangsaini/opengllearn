#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

// manage vertices
class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();


    void SetInstanceVAO(std::vector<glm::vec3>& InstanceData);
    void drawInstancedV(std::vector<glm::vec3>& InstanceData);
    void BindObj();
    void BindLight();

    const unsigned int getVBO() { return VBO; }
    const unsigned int getVAO() { return VAO; }
    const unsigned int getLightVAO() { return lightVAO; }

private:
    float vertices[36 * 11] = {
    // Position           // Normals            //Color     // TexCord
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    0, 0,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    1, 0,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    1, 1,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    1, 1,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    0, 1,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0, 0,    0, 0,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    0, 0,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    1, 0,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    1, 1,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    1, 1,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    0, 1,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0, 0,    0, 0,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 0,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 0,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 1,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 1,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 1,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 0,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 0,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 0,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 1,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    1, 1,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 1,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1, 0, 0,    0, 0,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    0, 0,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    1, 0,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    1, 1,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    1, 1,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    0, 1,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1, 0, 0,    0, 0,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    0, 0,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    1, 0,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    1, 1,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    1, 1,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    0, 1,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1, 0, 0,    0, 0
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int instanceVBO = 0;
    unsigned int lightVAO;
};


// Handle Vertex indicies;
class IndexBuffer {
public:
    IndexBuffer();
    ~IndexBuffer();

    void bind();

private:

    unsigned int indices[6];

    unsigned int IBO;
};

