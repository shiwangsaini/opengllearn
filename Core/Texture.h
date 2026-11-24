#pragma once

#include "Shaders/shaders.h"

struct Texture {
	Texture() : ID(-1), type("NULL") {};
	unsigned int ID;	// id no
	std::string type;	// name
	std::string path;	// name
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	GLint CreateTexture(const char* texturePath, const char* directory);
	void bind();
	void unbind();

	const GLint GetTextureID(const size_t& index);

private:
	std::vector<Texture> mTextures;

};

