#pragma once

#include "Texture.h"
#include "struct.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Model {
	
	class Mesh{
	public:
		Mesh(const std::vector<Buffers::VertexData>& vertex, const std::vector<unsigned int>& index, const std::vector<Texture>& textureData);
		~Mesh() {};

		std::vector<Buffers::VertexData>	vertices;
		std::vector<unsigned int>			indices;
		std::vector<Texture>				textures;

		void Draw(Shader& shader, const uint16_t& type) const;
		void DrawInstance(Shader& shader, const uint16_t& type) const;
		void SetInstance(const std::vector<glm::vec3>& Data);
		void SetInstance(const glm::vec3& Data);
		void SetMaterial(const Material& mat);
		void ApplyMaterial(Shader& shader, const uint16_t& type) const;
		void SetBoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds);
		const BoundingBox& GetBoundBox() const { return boundBox; }

	private:
		unsigned int VBO, VAO, IBO, InstanceBO;
		Material mMaterial;
		GLsizei InstanceCount = 0;
		void SetMesh();
		BoundingBox boundBox;
	};

	class Model {
	public:
		Model() { mMeshes.clear(); };
		~Model() { mMeshes.clear(); };
		
		void SetInstance(const std::vector<glm::vec3>& InstancePos);
		void SetInstance(const glm::vec3& InstancePos);
		void ProcessNode(aiNode* Node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
		const std::vector<Mesh>& GetMeshes() const { return mMeshes; }
		const BoundingBox& GetBoundBox() const { return boundBox; }

	private:
		std::vector<Texture> LoadedTexture;
		std::vector<Mesh> mMeshes;
		std::string directory;
		BoundingBox boundBox;
	};

	class Manager {
	public:
		Manager() {};
		~Manager() {};

		void LoadModel(const std::string& path);
		void Draw(Shader& shader, const uint16_t& type) const;
		void DrawInstance(Shader& shader, const uint16_t& type) const;
		void DrawInstance(Shader& shader, const uint16_t& type, const unsigned int& modelNo);
		void InstanceModel(const glm::vec3& InstancePos, const uint16_t& meshID);
		void InstanceModel(const std::vector<glm::vec3>& InstancePos, const uint16_t& meshID);
		const BoundingBox& GetModelBoundingBox(const size_t& modelId) const { return mModels[modelId].GetBoundBox(); }
		const uint16_t& GetModleID(const char* ModelName) const { return 0; }
	private:
		std::vector<Model> mModels;
	};
}
