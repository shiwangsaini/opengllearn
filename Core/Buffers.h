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

    void bind();

private:

    unsigned int VBO;
    unsigned int VAO;
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

