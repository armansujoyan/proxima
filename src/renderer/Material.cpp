//
// Created by Arman Sujoyan on 4/3/20.
//

#include "Material.h"

Material::Material():
    m_key_diffuse(glm::vec3(0)),
    m_key_specular(glm::vec3(0)),
    m_key_ambient(glm::vec3(0)),
    m_diffuse_texture(nullptr),
    m_specular_texture(nullptr),
    m_shader(nullptr) {};

Material::Material(MaterialMeta &meta) : m_key_diffuse(meta.Kd), m_key_ambient(meta.Ka), m_key_specular(meta.Ks) {
    if (!meta.map_Kd.empty()) {
        m_diffuse_texture = new Texture(meta.map_Kd, true, 0);
    } else {
        m_diffuse_texture = nullptr;
    }

    if (!meta.map_Ks.empty()) {
        m_specular_texture = new Texture(meta.map_Ks, true, 1);
    } else {
        m_specular_texture = nullptr;
    }

    m_shader = new Shader(ROOT_DIR "/resources/shaders/default.vs", ROOT_DIR "/resources/shaders/default.fs");
}

void Material::addDiffuseMap(Texture *diffuse) {
    m_diffuse_texture = diffuse;
}

void Material::addSpecularMap(Texture *specular) {
    m_specular_texture = specular;
}

void Material::addShader(Shader *shader) {
    m_shader = shader;
}

void Material::attach() {
    if (m_diffuse_texture != nullptr) {
        m_diffuse_texture->bind();
    }

    if (m_specular_texture != nullptr) {
        m_specular_texture->bind();
    }
}

void Material::detach() {
    if (m_diffuse_texture != nullptr) {
        m_diffuse_texture->unbind();
    }

    if (m_specular_texture != nullptr) {
        m_specular_texture->unbind();
    }
}
