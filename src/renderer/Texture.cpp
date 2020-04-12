//
// Created by Arman Sujoyan on 3/29/20.
//

#include "Texture.h"

Texture::Texture(const std::string &texPath, const bool flip, const unsigned int texUnitId) :
texUnitId(texUnitId)
{
    GLCall(glad_glGenTextures(1, &textureId));
    GLCall(glad_glBindTexture(GL_TEXTURE_2D, textureId));

    createSampler();
    loadTexture(texPath, flip);

    GLCall(glad_glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
    GLCall(glad_glDeleteTextures(1, &textureId));
    GLCall(glad_glDeleteSamplers(1, &samplerId));
}

void Texture::createSampler() {
    GLCall(glad_glGenSamplers(1, &samplerId));

    GLCall(glad_glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glad_glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glad_glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glad_glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

void Texture::loadTexture(const std::string &texPath, const bool flip) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum sourceType = getTextureSourceType(nrChannels);
        GLCall(glad_glTexImage2D(GL_TEXTURE_2D, 0, sourceType, width, height, 0, sourceType, GL_UNSIGNED_BYTE, data));
        GLCall(glad_glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::setParameter(GLenum parameter, GLenum value) const {
    GLCall(glad_glSamplerParameteri(samplerId, parameter, value));
}

void Texture::bind() const {
    GLCall(glad_glBindSampler(texUnitId, samplerId));
    GLCall(glad_glActiveTexture(GL_TEXTURE0 + texUnitId));
    GLCall(glad_glBindTexture(GL_TEXTURE_2D, textureId));
}

void Texture::bindSampler() {
    GLCall(glad_glBindSampler(texUnitId, samplerId));
}

void Texture::unbind() const {
    GLCall(glad_glBindSampler(texUnitId, 0));
    GLCall(glad_glActiveTexture(GL_TEXTURE0 + texUnitId));
    GLCall(glad_glBindTexture(GL_TEXTURE_2D, 0));
}

GLenum Texture::getTextureSourceType(int channels) {
    GLenum sourceType;
    switch (channels) {
        case 1:
            sourceType = GL_RED;
            break;
        case 2:
            sourceType = GL_RG;
            break;
        case 3:
            sourceType = GL_RGB;
            break;
        case 4:
            sourceType = GL_RGBA;
            break;
        default:
            sourceType = GL_RGB;
            break;
    }

    return sourceType;
}