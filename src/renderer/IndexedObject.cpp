//
// Created by Arman Sujoyan on 2/8/20.
//

#include "IndexedObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "../utils/StringUtils.h"

IndexedObject::~IndexedObject() = default;

// TODO: Add possibility to generate normals when they are not present
IndexedObject::IndexedObject(const std::string &path) {
    std::fstream objectFile;
    objectFile.open(path);

    std::string line;

    if (objectFile.is_open()) {
        while(objectFile.good()) {
            getline(objectFile, line);

            const char* lineCstr = line.c_str();
            unsigned int lineLength = line.size();

            if(lineLength < 2)
                continue;

            switch(lineCstr[0]) {
                case 'v':
                    if(lineCstr[1] == 'n')
                        this->m_raw_normals.push_back(ParseVec3(line.substr(2)));
                    else if (lineCstr[1] == 't')
                        this->m_raw_textures.push_back(ParseVec2(line.substr(2)));
                    else if (lineCstr[1] == ' ' || lineCstr[1] == '\t')
                        this->m_raw_positions.push_back(ParseVec3(line.substr(1)));
                    break;
                case 'f':
                    ParseFace(line.substr(2));
                    break;
                default: break;
            };
        }
    } else {
        std::cout << "Error: Cannot open the file for loading the model" << std::endl;
    }

    GenerateIndexedObject();
}

glm::vec3 IndexedObject::ParseVec3(const std::string &line) {
    std::stringstream ss(line);
    float parsedValues[3];

    for (float &parsedValue : parsedValues) {
        ss >> parsedValue;
    }

    return glm::vec3(
            parsedValues[0],
            parsedValues[1],
            parsedValues[2]);
}

glm::vec2 IndexedObject::ParseVec2(const std::string &line) {
    std::stringstream ss(line);
    float parsedValues[3];

    for (int i = 0; i < 2; ++i) {
        ss >> parsedValues[i];
    }

    return glm::vec2(
            parsedValues[0],
            parsedValues[1]);
}

void IndexedObject::ParseFace(const std::string &line) {
    std::vector<std::string> tokens;
    split(line, tokens, ' ');

    for(const auto& token: tokens) {
        std::vector<std::string> coupled_index_elements;
        split(token, coupled_index_elements, '/');
        IObject current{};
        current.m_position_index = std::stoi(coupled_index_elements[0]);
        current.m_texture_index = std::stoi(coupled_index_elements[1]);
        current.m_normal_index = std::stoi(coupled_index_elements[2]);

        this->m_coupled_indices.push_back(current);
    }
}

static bool CompareOBJIndexPtr(const IObject* a, const IObject* b)
{
    return a->m_position_index < b->m_position_index;
}

void IndexedObject::GenerateIndexedObject() {
    unsigned int index = 0;

    std::vector<std::pair<IObject, unsigned int>> lookup;

    unsigned int existingIndex;

    for (int i = 0; i < this->m_coupled_indices.size(); ++i) {
        IObject current = m_coupled_indices[i];

        existingIndex = findExistingIndex(lookup, &current);
        if(existingIndex == (unsigned int)-1) {
            this->m_indices.push_back(index);
            this->m_indexed_positions.push_back(this->m_raw_positions[current.m_position_index - 1]);
            this->m_indexed_textures.push_back(this->m_raw_textures[current.m_texture_index - 1]);
            this->m_indexed_normals.push_back(this->m_raw_normals[current.m_normal_index - 1]);
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
