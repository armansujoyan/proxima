//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_INDEXEDGEOMETRY_H
#define PROXIMA_INDEXEDGEOMETRY_H

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

struct Triangle {
    glm::vec3 vertex1;
    glm::vec3 vertex2;
    glm::vec3 vertex3;
    glm::vec3 normal;
};

class IndexedGeometry {
public:
    explicit IndexedGeometry(const std::vector<glm::vec3> &raw_positions, const std::vector<glm::vec2> &raw_textures,
                             const std::vector<glm::vec3> &raw_normals, const std::vector<IObject> &coupled_indices);
    ~IndexedGeometry();

    std::vector<glm::vec3> m_indexed_positions;
    std::vector<glm::vec2> m_indexed_textures;
    std::vector<glm::vec3> m_indexed_normals;
    std::vector<unsigned int> m_indices;
    std::vector<Triangle> m_triangles;
private:
    void GenerateIndexedObject (
            const std::vector<glm::vec3> &raw_positions,
            const std::vector<glm::vec2> &raw_textures,
            const std::vector<glm::vec3> &raw_normals,
            const std::vector<IObject> &coupled_indices);

    static unsigned int findExistingIndex (
            const std::vector<std::pair<IObject, unsigned int>>& lookup,
            IObject* current);
};


#endif //PROXIMA_INDEXEDGEOMETRY_H
