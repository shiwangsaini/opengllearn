#pragma once

#include "VertexBuffer.h"

class Texture
{
public:
	Texture() {}
	Texture(const int renderID);
	~Texture();

	void CreateTexture(const char* texturePath);
	void bind();
	void unbind();

private:
	unsigned int m_RenderID = 0;
	std::vector<unsigned int> textureID;

protected:
};

