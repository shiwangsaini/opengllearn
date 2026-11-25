#include "ModelLoad.h"

namespace Model {

	TextureManager mTexturManager;

	Mesh::Mesh(const std::vector<Buffers::VertexData>& vertex, const std::vector<unsigned int>& index, const std::vector<Texture>& textureData)
	{
		this->vertices = std::vector<Buffers::VertexData>(vertex);
		this->indices =  index;
		this->textures = textureData;

		glGenBuffers(1, &InstanceBO);

		SetMesh();
	}
	
	void Mesh::Draw(Shader& shader, const uint16_t& type) const
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		unsigned int normalNr = 1;
		bool hasSpec = false, hasNormal = false;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			const std::string& name = textures[i].type;
			std::string number;

			if (name == "texture_diffuse")       number = std::to_string(diffuseNr++);
			else if (name == "texture_specular") { number = std::to_string(specularNr++); hasSpec = true; }
			else if (name == "texture_normal") { number = std::to_string(normalNr++);   hasNormal = true; }

			shader.setUniform1Int(("material." + name + number).c_str(), i, type);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
		shader.setUniform1Int("hasSpecular", hasSpec, type);
		shader.setUniform1Int("hasNormalMap", hasNormal, type);
		
		ApplyMaterial(shader, type);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::DrawInstance(Shader& shader, const uint16_t& type) const
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		unsigned int normalNr = 1;
		bool hasSpec = false, hasNormal = false;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			const std::string& name = textures[i].type;
			std::string number;

			if (name == "texture_diffuse")       number = std::to_string(diffuseNr++);
			else if (name == "texture_specular") { number = std::to_string(specularNr++); hasSpec = true; }
			else if (name == "texture_normal") { number = std::to_string(normalNr++);   hasNormal = true; }

			shader.setUniform1Int(("material." + name + number).c_str(), i, type);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
		shader.setUniform1Int("hasSpecular", hasSpec, type);
		shader.setUniform1Int("hasNormalMap", hasNormal, type);

		ApplyMaterial(shader, type);

		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr, InstanceCount);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::SetInstance(const std::vector<glm::vec3>& Data)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, InstanceBO);
		glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(glm::vec3), Data.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(5, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		InstanceCount = static_cast<GLsizei>(Data.size());
	}

	void Mesh::SetInstance(const glm::vec3& Data)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, InstanceBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &Data, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(5, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		InstanceCount = 1;
	}
	
	void Mesh::ApplyMaterial(Shader& shader, const uint16_t& type) const
	{
		shader.setUniform1Int("useMaterialColor", mMaterial.useColor, type);

		if (mMaterial.useColor) {
			shader.setUniformVec3("material.diffuse", mMaterial.diffuse, type);
			shader.setUniformVec3("material.specular", mMaterial.specular, type);
			shader.setUniformVec3("material.ambient", mMaterial.ambient, type);
			shader.setUniform1Float("material.shininess", mMaterial.shininess, type);
		}
		else {
			shader.setUniformVec3("material.ambient", glm::vec3(1.0f), type);
		}
	}

	void Mesh::SetMaterial(const Material& mat)
	{
		mMaterial = mat;
	}

	void Mesh::SetBoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds)
	{
		this->boundBox.minBounds = minBounds;
		this->boundBox.maxBounds = maxBounds;
	}


	void Mesh::SetMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Buffers::VertexData), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Buffers::VertexData), (void*)0);
		// Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Buffers::VertexData), (void*)offsetof(Buffers::VertexData, Normal));
		//
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Buffers::VertexData), (void*)offsetof(Buffers::VertexData, UVs));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Buffers::VertexData), (void*)offsetof(Buffers::VertexData, Tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Buffers::VertexData), (void*)offsetof(Buffers::VertexData, Bitangent));

		glBindVertexArray(0);
	}

	void Manager::Draw(Shader& shader, const uint16_t& type) const
	{
		for (auto& model: mModels) {
			for (const auto& mesh : model.GetMeshes())
				mesh.Draw(shader, type);
		}
	}
	void Manager::DrawInstance(Shader& shader, const uint16_t& type) const
	{
		for(const auto& model : mModels)
			for(const auto& mesh : model.GetMeshes())
				mesh.DrawInstance(shader, type);
	}
	void Manager::DrawInstance(Shader& shader, const uint16_t& type, const unsigned int& modelNo)
	{
		for(const auto& mesh : mModels[modelNo].GetMeshes())
			mesh.DrawInstance(shader, type);
	}
	void Manager::LoadModel(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path,
			aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_GenBoundingBoxes |
			aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ASSIMP:" << import.GetErrorString() << std::endl;
			return;
		}
		mModels.push_back(Model());
		auto& model = mModels.back();
		model.ProcessNode(scene->mRootNode, scene);
	}

	void Manager::InstanceModel(const std::vector<glm::vec3>& InstancePos, const uint16_t& meshID)
	{
		mModels[meshID].SetInstance(InstancePos);
	}

	void Manager::InstanceModel(const glm::vec3& InstancePos, const uint16_t& meshID)
	{
		mModels[meshID].SetInstance(InstancePos);
	}
	
	void Model::ProcessNode(aiNode* Node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[Node->mMeshes[i]];
			mMeshes.push_back(ProcessMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < Node->mNumChildren; i++) {
			ProcessNode(Node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Buffers::VertexData>	vertices;
		std::vector<unsigned int>			indices;
		std::vector<Texture>				textures;

		// Bounding box (AABB) tracking
		glm::vec3 maxBounds = glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z);
		glm::vec3 minBounds = glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z);

		boundBox.minBounds = glm::min(boundBox.minBounds, minBounds);
		boundBox.maxBounds = glm::max(boundBox.maxBounds, maxBounds);

		// filling vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Buffers::VertexData VD;
			glm::vec3 vertex{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			glm::vec3 normal{ mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z  };
			glm::vec2 texCord{0.0f,0.0f};
			if (mesh->mTextureCoords[0]) {
				texCord.x = mesh->mTextureCoords[0][i].x;
				texCord.y = mesh->mTextureCoords[0][i].y;
			}
			VD.Position = vertex;
			VD.Normal= normal;
			VD.UVs = texCord;
			
			if (mesh->HasTangentsAndBitangents()) {
				VD.Tangent = { mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z };
				VD.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			vertices.push_back(VD);
		}


		// filling indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Materials texture
		Material mat;
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			auto diffuseMap = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
			auto specularMap = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
			auto normalMap = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal");
			auto normalMap2 = LoadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");

			// OBJ "map_Bump" arrives as HEIGHT; other formats use NORMALS
			textures.insert(textures.end(), diffuseMap.begin(),  diffuseMap.end());
			textures.insert(textures.end(), specularMap.begin(), specularMap.end());
			textures.insert(textures.end(), normalMap.begin(),   normalMap.end());
			textures.insert(textures.end(), normalMap2.begin(),  normalMap2.end());

			if (textures.empty()) {
				mat.useColor = true;
				aiColor3D color(0.f);
				material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				mat.diffuse = glm::vec3(color.r, color.g, color.b);
				material->Get(AI_MATKEY_COLOR_AMBIENT,color);
				mat.ambient = glm::vec3(color.r, color.g, color.b);
				material->Get(AI_MATKEY_COLOR_SPECULAR, color);
				mat.specular = glm::vec3(color.r, color.g, color.b);
				float shine;
				material->Get(AI_MATKEY_SHININESS, shine);
				mat.shininess = shine;
			}
		}

		Mesh result = Mesh(vertices, indices, textures);
		result.SetBoundingBox(minBounds, maxBounds);
		result.SetMaterial(mat);
		return result;
	}
	std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < LoadedTexture.size(); j++)
			{
				if (std::strcmp(LoadedTexture[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(LoadedTexture[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.ID = mTexturManager.CreateTexture(str.C_Str(), directory.c_str());
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				LoadedTexture.push_back(texture); // add to loaded textures
			}
		}
		return textures;
	}

	void Model::SetInstance(const std::vector<glm::vec3>& InstancePos)
	{
		for (auto& mesh : mMeshes) {
			mesh.SetInstance(InstancePos);
		}
	}

	void Model::SetInstance(const glm::vec3& InstancePos)
	{
		for (auto& mesh : mMeshes) {
			mesh.SetInstance(InstancePos);
		}
	}
}