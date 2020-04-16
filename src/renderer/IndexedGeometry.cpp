//
// Created by Arman Sujoyan on 2/8/20.
//

#include "IndexedGeometry.h"
#include <map>

IndexedGeometry::~IndexedGeometry() = default;

IndexedGeometry::IndexedGeometry(const std::vector<glm::vec3> &raw_positions, const std::vector<glm::vec2> &raw_textures,
                                 const std::vector<glm::vec3> &raw_normals, const std::vector<IObject> &coupled_indices) {
    GenerateIndexedObject(raw_positions, raw_textures, raw_normals, coupled_indices);
}

static bool CompareOBJIndexPtr(const IObject* a, const IObject* b)
{
    return a->m_position_index < b->m_position_index;
}

void IndexedGeometry::GenerateIndexedObject(
        const std::vector<glm::vec3> &raw_positions,
        const std::vector<glm::vec2> &raw_textures,
        const std::vector<glm::vec3> &raw_normals,
        const std::vector<IObject> &coupled_indices) {
    unsigned int index = 0;
    unsigned int counter = 1;

    std::vector<std::pair<IObject, unsigned int>> lookup;

    unsigned int existingIndex;
    for (auto current : coupled_indices) {
        existingIndex = findExistingIndex(lookup, &current);
        if(existingIndex == (unsigned int)-1) {
            this->m_indices.push_back(index);
            this->m_indexed_positions.push_back(raw_positions[current.m_position_index - 1]);
            this->m_indexed_textures.push_back(raw_textures[current.m_texture_index - 1]);
            this->m_indexed_normals.push_back(raw_normals[current.m_normal_index - 1]);
            lookup.emplace_back(current, index);

            Triangle newTriangle = {};
            index++;
        } else {
            this->m_indices.push_back(existingIndex);
        }

        if (counter % 3 == 0) {
            Triangle newTriangle{};
            newTriangle.vertex1 =  m_indexed_positions[m_indices[counter - 3]];
            newTriangle.vertex2 =  m_indexed_positions[m_indices[counter - 2]];
            newTriangle.vertex3 =  m_indexed_positions[m_indices[counter - 1]];

            glm::vec3 edge1 = newTriangle.vertex2 - newTriangle.vertex1;
            glm::vec3 edge2 = newTriangle.vertex3 - newTriangle.vertex2;

            newTriangle.normal =  glm::normalize(glm::cross(edge1, edge2));

            m_triangles.push_back(newTriangle);
        }

        counter++;
    }
}

// TODO: Optimize this lookup and make it O(logn) via binary search
unsigned int IndexedGeometry::findExistingIndex(
        const std::vector<std::pair<IObject, unsigned int>>& lookup, IObject* current) {
    for(auto element: lookup) {
        IObject candidate = element.first;
        if (candidate.m_position_index == current->m_position_index &&
            candidate.m_texture_index == current->m_texture_index &&
            candidate.m_normal_index == current->m_normal_index) {
            return element.second;
        }
    }

    return -1;
}
