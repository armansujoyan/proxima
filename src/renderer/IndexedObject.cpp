//
// Created by Arman Sujoyan on 2/8/20.
//

#include "IndexedObject.h"
#include <map>

IndexedObject::~IndexedObject() = default;

IndexedObject::IndexedObject(const std::vector<glm::vec3> &raw_positions, const std::vector<glm::vec2> &raw_textures,
                             const std::vector<glm::vec3> &raw_normals, const std::vector<IObject> &coupled_indices) {
    GenerateIndexedObject(raw_positions, raw_textures, raw_normals, coupled_indices);
}

static bool CompareOBJIndexPtr(const IObject* a, const IObject* b)
{
    return a->m_position_index < b->m_position_index;
}

void IndexedObject::GenerateIndexedObject(
        const std::vector<glm::vec3> &raw_positions,
        const std::vector<glm::vec2> &raw_textures,
        const std::vector<glm::vec3> &raw_normals,
        const std::vector<IObject> &coupled_indices) {
    unsigned int index = 0;

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
            index++;
        } else {
            this->m_indices.push_back(existingIndex);
        }
    }
}

// TODO: Optimize this lookup and make it O(logn) via binary search
unsigned int IndexedObject::findExistingIndex(
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
