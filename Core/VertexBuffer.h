#pragma once

#include "Shaders/shaders.h"

class VertexBuffer
{
public:
    VertexBuffer() {}
    ~VertexBuffer();

    void bind();

private:

    float vertices[32] = {
        // positions                    // colors               // texture coords
         100.0f,   100.0f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f,   // top right
         100.0f,  -100.0f, 0.0f,       0.0f, 1.0f, 0.0f,       1.0f, 0.0f,   // bottom right
        -100.0f,  -100.0f, 0.0f,       0.0f, 0.0f, 1.0f,       0.0f, 0.0f,   // bottom left
        -100.0f,   100.0f, 0.0f,       1.0f, 1.0f, 0.0f,       0.0f, 1.0f    // top left 
    };

    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int IBO;
};

