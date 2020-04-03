//
// Created by Arman Sujoyan on 4/3/20.
//

#include "Material.h"

Material::Material() = default;

void Material::addDiffuseMap(Texture &diffuse) {
    m_diffuse_texture = diffuse;
}

void Material::addSpecularMap(Texture &specular) {
    m_specular_texture = specular;
}

void Material::addShader(Shader &shader) {
    m_shader = shader;
}
