//
// Created by Arman Sujoyan on 3/29/20.
//

#ifndef PROXIMA_TEXTURE_H
#define PROXIMA_TEXTURE_H

#include <iostream>
#include <utils/Debug.h>
#include <stb_image.h>

class Texture {
public:
    Texture(const char* texPath, const bool flip = true, const unsigned int texUnitId = 0, const GLenum sourceType = GL_RGB);
    ~Texture();
    void setParameter(GLenum parameter, GLenum value) const;
    void bind() const;
private:
    unsigned int textureId;
    unsigned int samplerId;
    unsigned int texUnitId;
    void createSampler();
    void loadTexture(const char* texPath, const GLenum sourceType, const bool flip);
};

#endif //PROXIMA_TEXTURE_H
