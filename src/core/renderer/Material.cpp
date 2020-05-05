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

Material::Material(const Material &M):
    m_material_name(M.m_material_name),
    m_key_diffuse(M.m_key_diffuse),
    m_key_ambient(M.m_key_ambient),
    m_key_specular(M.m_key_specular) {
    if (M.m_diffuse_texture != nullptr) {
        m_diffuse_texture = new Texture(*(M.m_diffuse_texture));
    } else {
        m_diffuse_texture = nullptr;
    }

    if (M.m_specular_texture != nullptr) {
        m_specular_texture = new Texture(*(M.m_specular_texture));
    } else {
        m_specular_texture = nullptr;
    }

    if (M.m_shader != nullptr) {
        m_shader = new Shader(*(M.m_shader));
    } else {
        m_shader = nullptr;
    }
}

Material &Material::operator=(const Material &M) {
    if ( this == &M) {
        return *this;
    } else {
        delete m_diffuse_texture;
        delete m_specular_texture;
        delete m_shader;

        if (M.m_diffuse_texture != nullptr) {
            m_diffuse_texture = new Texture(*(M.m_diffuse_texture));
        } else {
            m_diffuse_texture = nullptr;
        }

        if (M.m_specular_texture != nullptr) {
            m_specular_texture = new Texture(*(M.m_specular_texture));
        } else {
            m_specular_texture = nullptr;
        }

        if (M.m_shader != nullptr) {
            m_shader = new Shader(*(M.m_shader));
        } else {
            m_shader = nullptr;
        }

        return *this;
    }
}

Material::Material(MaterialMeta &meta) : m_key_diffuse(meta.Kd), m_key_ambient(meta.Ka), m_key_specular(meta.Ks),
    m_material_name(meta.name) {
    if (!meta.map_Kd.empty()) {
        m_diffuse_texture = new Texture(meta.map_Kd, true, 0);
    }

    if (!meta.map_Ks.empty()) {
        m_specular_texture = new Texture(meta.map_Ks, true, 1);
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

Material::~Material() {
    delete m_diffuse_texture;
    delete m_specular_texture;
    delete m_shader;
}

Shader* Material::getShader() {
    return m_shader;
}
