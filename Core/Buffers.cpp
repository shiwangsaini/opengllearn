#include "Buffers.h"

#include <iostream>

namespace Buffers {
    Vertex::Vertex()
        : VBO(0), VAO(0)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        verticies.clear();
    }

    Vertex::~Vertex()
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Vertex::AddVertex(const Layout& layout)
    {
        Bind();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        const std::vector<Elements>& elements = layout.GetElements();
        unsigned int offset = 0;
        for (int i = 0; i < elements.size(); i++) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*)offset);
            offset += elements[i].count * Elements::GetTypeSize(elements[i].type);
        }
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Vertex::SetInstanceVAO(const std::vector<glm::vec3>& InstanceData)
    {
        Bind();
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, InstanceData.size() * sizeof(glm::vec3), InstanceData.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(3, 1);
        UnBind();
    }

    void Vertex::drawInstancedV(const std::vector<glm::vec3>& InstanceData)
    {
        Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, static_cast<GLsizei>(InstanceData.size()));
        UnBind();
    }
    void Vertex::drawInstancedV(const std::vector<glm::vec3>& InstanceData, const uint16_t& size)
    {
        Bind();
        glDrawElementsInstanced(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(InstanceData.size()));
        UnBind();
    }

    void Vertex::Bind()
    {
        glBindVertexArray(VAO);
    }

    void Vertex::UnBind()
    {
        glBindVertexArray(0);
    }

    // vertexdata: 1.vertex points, 2.normals and 3.texture cordinate or Model data. usage: gl Drawtype
    void Vertex::AddDrawData(const std::vector<float>& vertexData, const int& usage){ 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Index::Index()
        : IBO(0)
    {
        glGenBuffers(1, &IBO);
        indices.clear();
    }

    Index::~Index()
    {
        glDeleteBuffers(1, &IBO);
    }

    void Index::AddIndex(const std::vector<unsigned int>& index)
    {
        indices.insert(indices.end(), index.begin(), index.end());
    }

    void Index::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }
}