//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_INDEXEDGEOMETRY_H
#define PROXIMA_INDEXEDGEOMETRY_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

struct IObject {
    unsigned int m_position_index;
    unsigned int m_texture_index;
    unsigned int m_normal_index;

    bool operator==(const IObject &other) const
    { return (m_position_index == other.m_position_index
              && m_texture_index == other.m_texture_index
              && m_normal_index == other.m_normal_index);
    }
};

struct IObjectHasher
{
    std::size_t operator()(const IObject& k) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<unsigned int>()(k.m_position_index)
                 ^ (hash<unsigned int>()(k.m_texture_index) << 1)) >> 1)
               ^ (hash<unsigned int>()(k.m_normal_index) << 1);
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
            const std::unordered_map<IObject, unsigned int, IObjectHasher>& lookup,
            IObject* current);
};


#endif //PROXIMA_INDEXEDGEOMETRY_H
