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
    explicit Texture(const std::string &texPath, bool flip = true,
            unsigned int texUnitId = 0);
    Texture(): textureId(0), samplerId(0), texUnitId(0) {};
    ~Texture();
    void setParameter(GLenum parameter, GLenum value) const;
    void bind() const;
    void unbind() const;
private:
    unsigned int textureId{};
    unsigned int samplerId{};
    unsigned int texUnitId{};

    void createSampler();
    static void loadTexture(const std::string &texPath, bool flip);

    void bindSampler();

    static GLenum getTextureSourceType(int channels);
};

#endif //PROXIMA_TEXTURE_H
