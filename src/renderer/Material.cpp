//
// Created by Arman Sujoyan on 4/3/20.
//

#include "Material.h"

Material::Material() = default;

Material::Material(MaterialMeta &meta) : m_key_diffuse(meta.Kd), m_key_ambient(meta.Ka), m_key_specular(meta.Ks) {
    if (!meta.map_Kd.empty()) {
        m_diffuse_texture = Texture(meta.map_Kd.c_str(), true, 0, GL_RGB);
    }

    if (!meta.map_Ks.empty()) {
        m_specular_texture = Texture(meta.map_Kd.c_str(), true, 1, GL_RGB);
    }
}

void Material::addDiffuseMap(Texture &diffuse) {
    m_diffuse_texture = diffuse;
}

void Material::addSpecularMap(Texture &specular) {
    m_specular_texture = specular;
}

void Material::addShader(Shader &shader) {
    m_shader = shader;
}
