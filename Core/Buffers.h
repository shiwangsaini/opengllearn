#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

namespace Buffers {

    struct VertexData {
        glm::vec3 Position; // position
        glm::vec3 Normal;   // normal map
        glm::vec2 UVs;      // texture CoOrdinates

        glm::vec3 Tangent;
        glm::vec3 Bitangent;


    };

    struct Elements {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;

        static unsigned int GetTypeSize(int type) {
            switch (type) {
            case GL_FLOAT:           return 4;
            case GL_UNSIGNED_INT:    return 4;
            case GL_UNSIGNED_BYTE:   return 1;
            }
            throw "Buffer Element Type is not Sized\n";
        }
    };

    class Layout {
    public:
        Layout() :mStride(0) { mElements.clear(); };
        ~Layout() {};

        template<typename T>
        void Push(unsigned int count) { static_assert(false); };
        template<>
        void Push<float>(unsigned int count) {
            mElements.push_back({ GL_FLOAT, count, GL_FALSE });
            mStride += count * Elements::GetTypeSize(GL_FLOAT);
        }
        template<>
        void Push<unsigned int>(unsigned int count) {
            mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
            mStride += count * Elements::GetTypeSize(GL_UNSIGNED_INT);
        }
        template<>
        void Push<unsigned char>(unsigned int count) {
            mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
            mStride += count * Elements::GetTypeSize(GL_UNSIGNED_BYTE);
        }

        inline const std::vector<Elements>& GetElements() const { return mElements; }
        inline unsigned int GetStride() const { return mStride; }

    private:
        std::vector<Elements> mElements;
        unsigned int mStride;
    };

    // manage vertices
    class Vertex
    {
    public:
        Vertex();
        ~Vertex();

        void SetInstanceVAO(const std::vector<glm::vec3>& InstanceData);
        void drawInstancedV(const std::vector<glm::vec3>& InstanceData);
        void drawInstancedV(const std::vector<glm::vec3>& InstanceData, const uint16_t& size);
        void Bind();
        void UnBind();

        void AddVertex(const Layout& layout);
        void AddDrawData(const std::vector<float>& vertexData, const int& usage);

        const unsigned int getVBO() { return VBO; }
        const unsigned int getVAO() { return VAO; }

    private:
        std::vector<float> verticies;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int instanceVBO = 0;
    };


    // Handle Vertex indicies;
    class Index {
    public:
        Index();
        ~Index();

        void AddIndex(const std::vector<unsigned int>& index);
        void bind();

    private:

        std::vector<unsigned int> indices;
        unsigned int IBO;
    };
}