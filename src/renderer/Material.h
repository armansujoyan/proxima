//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_MATERIAL_H
#define PROXIMA_MATERIAL_H

#include <string>
#include <helpers/RootDir.h>
#include "Texture.h"
#include "Shader.h"

class Material {
public:
    Material();
    Material(float key_diffuse, float key_ambient, float key_specular):
        m_key_diffuse(key_diffuse), m_key_ambient(key_ambient), m_key_specular(key_specular) {};
    void addDiffuseMap(Texture& diffuse);
    void addSpecularMap(Texture& specular);
    void addShader(Shader& shader);
private:
    float m_key_diffuse{};
    float m_key_ambient{};
    float m_key_specular{};
    Texture m_diffuse_texture;
    Texture m_specular_texture;
    Shader m_shader;
};


#endif //PROXIMA_MATERIAL_H
