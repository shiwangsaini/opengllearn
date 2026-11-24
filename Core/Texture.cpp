#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureManager::TextureManager()
{
    stbi_set_flip_vertically_on_load(true);
}

TextureManager::~TextureManager()
{
    unbind();
    if (!mTextures.empty())
        for(auto& t : mTextures)
            glDeleteTextures(1, &t.ID);
}

GLint TextureManager::CreateTexture(const char* texturePath, const char* directory)
{
    std::string Path = "Core/Assets/" + std::string(texturePath);
    Texture texture;
    GLuint& id = texture.ID;
    texture.type = texturePath;
    texture.path = directory;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(Path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;

        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;

        case 2:
            format = GL_RG;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            std::cout << "Unsupported channel count: " << nrChannels << '\n';
            throw;
            stbi_image_free(data);
            return -1;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "\nFailed to load texture" << std::endl;
    }
    stbi_image_free(data);

    mTextures.push_back(texture);
    return id;
}

void TextureManager::bind()
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
        glBindTexture(GL_TEXTURE_2D, mTextures[i].ID);
    }
}

void TextureManager::unbind()
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

const GLint TextureManager::GetTextureID(const size_t& index)
{
    return mTextures[index].ID;
}
