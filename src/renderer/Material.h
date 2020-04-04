//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_MATERIAL_H
#define PROXIMA_MATERIAL_H

#include <string>
#include <helpers/RootDir.h>
#include "Texture.h"
#include "Shader.h"

struct MaterialMeta {
    std::string name;
    std::string map_Kd;
    std::string map_Ks;
    glm::vec3 Kd;
    glm::vec3 Ka;
    glm::vec3 Ks;
};

class Material {
public:
    Material();
    Material(glm::vec3 key_diffuse, glm::vec3 key_ambient, glm::vec3 key_specular):
        m_key_diffuse(key_diffuse), m_key_ambient(key_ambient), m_key_specular(key_specular) {};
    explicit Material(MaterialMeta &meta);
    void addDiffuseMap(Texture& diffuse);
    void addSpecularMap(Texture& specular);
    void addShader(Shader& shader);
private:
    glm::vec3 m_key_diffuse{};
    glm::vec3 m_key_ambient{};
    glm::vec3 m_key_specular{};
    Texture m_diffuse_texture;
    Texture m_specular_texture;
    Shader m_shader;
};


#endif //PROXIMA_MATERIAL_H
