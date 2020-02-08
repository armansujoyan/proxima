//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_INDEXEDOBJECT_H
#define PROXIMA_INDEXEDOBJECT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct IObject {
    unsigned int m_position_index;
    unsigned int m_texture_index;
    unsigned int m_normal_index;

    inline bool operator<(const IObject& rhs) const {
        return std::tie(m_position_index, m_normal_index, m_texture_index)
        < std::tie(rhs.m_position_index, rhs.m_normal_index, rhs.m_texture_index);
    }
};

class IndexedObject {
public:
    explicit IndexedObject(const std::string &path);
    ~IndexedObject();

    std::vector<glm::vec3> m_indexed_positions;
    std::vector<glm::vec2> m_indexed_textures;
    std::vector<glm::vec3> m_indexed_normals;
    std::vector<unsigned int> m_indices;
private:
    std::vector<glm::vec3> m_raw_positions;
    std::vector<glm::vec2> m_raw_textures;
    std::vector<glm::vec3> m_raw_normals;

    glm::vec3 ParseVec3(const std::string& line);
    glm::vec2 ParseVec2(const std::string& line);
    void ParseFace(const std::string& line);
    void GenerateIndexedObject();

    unsigned int findExistingIndex(const std::vector<std::pair<IObject, unsigned int>>& lookup, IObject* current);

    std::vector<IObject> m_coupled_indices;
};


#endif //PROXIMA_INDEXEDOBJECT_H
