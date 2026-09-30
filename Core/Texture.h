#pragma once

#include "Shaders/shaders.h"

class Texture
{
public:
	Texture();
	~Texture();

	void CreateTexture(const char* texturePath);
	void bind();
	void unbind();

	const GLint GetTextureID(const size_t& index);

private:
	std::vector<unsigned int> textureID;

protected:
};

