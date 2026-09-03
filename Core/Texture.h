#pragma once

#include "Shaders/shaders.h"

class Texture
{
public:
	Texture() {}
	Texture(unsigned int* renderID);
	~Texture();

	void CreateTexture(const char* texturePath);
	void bind();
	void unbind();

private:
	unsigned int* m_RenderID = nullptr;
	std::vector<unsigned int> textureID;

protected:
};

