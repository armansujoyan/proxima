//
// Created by Arman Sujoyan on 4/3/20.
//

#include <renderer/Material.h>
#include <utils/StringUtils.h>
#include <utils/Logging.h>
#include "OBJLoader.h"

#include <map>

struct PointGroup {
    std::string m_name;
    std::string m_material_name;
    std::vector<IObject> m_coupled_indices;
};

std::vector<Mesh*> OBJLoader::load(const std::string &path) {
    std::vector<GeometryMaterialPair> geometries = parseGeometry(path);
    std::map<std::string, Material> materialsMap = parseMaterials(path);
    std::vector<Mesh*> object_meshes;

    if (!materialsMap.empty()) {
        for(const auto& geometry: geometries) {
            std::string materialName = geometry.first;
            Material currentMeshMaterial = materialsMap[materialName];
            object_meshes.push_back(new Mesh(geometry.second, currentMeshMaterial));
        }
    } else {
        for(const auto& geometry: geometries) {
            object_meshes.push_back(new Mesh(geometry.second));
        }
    }

    return object_meshes;
}

std::vector<GeometryMaterialPair> OBJLoader::parseGeometry(const std::string &path) {
    std::vector<glm::vec3> raw_vertices, raw_normals;
    std::vector<glm::vec2> raw_textures;

    std::vector<GeometryMaterialPair> mesh_geometries;
    std::vector<PointGroup> point_groups;
    PointGroup currentGroup = PointGroup();
    bool isFirstGroup = true;
    std::string lastMaterial;

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

            if(line.find("usemtl") == 0) {
                lastMaterial = parseObjString(line);
            }

            switch(lineCstr[0]) {
                case 'v':
                    if(lineCstr[1] == 'n')
                        raw_normals.push_back(ParseVec3(line.substr(2)));
                    else if (lineCstr[1] == 't')
                        raw_textures.push_back(ParseVec2(line.substr(2)));
                    else if (lineCstr[1] == ' ' || lineCstr[1] == '\t')
                        raw_vertices.push_back(ParseVec3(line.substr(1)));
                    break;
                case 'f':
                    ParseFace(line.substr(2), currentGroup.m_coupled_indices);
                    break;
                case 'g':
                    if(lineCstr[1] == ' ') {
                        if (isFirstGroup) {
                            currentGroup.m_name = parseObjString(line);
                            isFirstGroup = false;
                        } else {
                            currentGroup.m_material_name = lastMaterial;
                            point_groups.push_back(currentGroup);
                            currentGroup = PointGroup();
                            currentGroup.m_name = parseObjString(line);
                        }
                    }
                default: break;
            };
        }
    } else {
        std::cout << "Error: Cannot open the file for loading the model" << std::endl;
    }

    currentGroup.m_material_name = lastMaterial;
    point_groups.push_back(currentGroup);

    mesh_geometries.reserve(point_groups.size());
    for(const auto& pointGroup: point_groups) {
        auto* currentGeometry = new IndexedGeometry(
                raw_vertices, raw_textures, raw_normals, pointGroup.m_coupled_indices);
        GeometryMaterialPair currentPair = GeometryMaterialPair(pointGroup.m_material_name, currentGeometry);
        mesh_geometries.push_back(currentPair);
    }

    return mesh_geometries;
}

std::map<std::string, Material> OBJLoader::parseMaterials(const std::string &objectPath) {
    std::string mtlPath = getMaterialPathFromObjectFile(objectPath);
    std::map<std::string, Material> materialsMap;
    if (!mtlPath.empty()) {
         materialsMap = getMaterialsMap(mtlPath, objectPath);
    }

    return materialsMap;
}

std::string OBJLoader::getMaterialPathFromObjectFile(const std::string &objectFilePath) {
    std::fstream objectFile;
    objectFile.open(objectFilePath);
    std::string materialFilePath;

    if (objectFile.is_open()) {
        materialFilePath = getMaterialFilePath(objectFile, objectFilePath);
    } else {
        printUnhandledFileMessage();
    }

    return materialFilePath;
}

std::map<std::string, Material>
OBJLoader::getMaterialsMap(const std::string &materialFilePath, const std::string &objectFilePath) {
    std::fstream mtlFile;
    mtlFile.open(materialFilePath);

    std::map<std::string, Material> materialsMap;
    MaterialMeta currentMaterialMeta;
    std::string line;

    if (mtlFile.is_open()) {
        bool isFirstMaterial = true;
        while(mtlFile.good()) {
            getline(mtlFile, line);

            unsigned int lineLength = line.size();
            size_t lastTokenPosition;

            if (lineLength < 2)
                continue;

            if (line.find("newmtl") != std::string::npos) {
                if (isFirstMaterial) {
                    isFirstMaterial = false;
                } else {
                    addMaterialFromMetaToMap(materialsMap, currentMaterialMeta);
                }
                currentMaterialMeta = MaterialMeta();
                currentMaterialMeta.name = getMaterialName(line);
                continue;
            }

            lastTokenPosition = line.find("Kd");
            if (lastTokenPosition == 0) {
                currentMaterialMeta.Kd = ParseVec3(line.substr(2));
                continue;
            }

            lastTokenPosition = line.find("Ka");
            if (lastTokenPosition == 0) {
                currentMaterialMeta.Ka = ParseVec3(line.substr(2));
            }

            lastTokenPosition = line.find("Ks");
            if (lastTokenPosition == 0) {
                currentMaterialMeta.Ks = ParseVec3(line.substr(2));
            }

            if (line.find("map_Kd") != std::string::npos) {
                currentMaterialMeta.map_Kd = getRelativePath(objectFilePath, parseObjString(line));
                continue;
            }

            if (line.find("map_Ks") != std::string::npos) {
                currentMaterialMeta.map_Ks = getRelativePath(objectFilePath, parseObjString(line));
                continue;
            }
        }
        addMaterialFromMetaToMap(materialsMap,currentMaterialMeta);
    } else {
        std::cout << "Error: Cannot open the file for loading the material" << std::endl;
    }

    return materialsMap;
}

void OBJLoader::addMaterialFromMetaToMap(std::map<std::string, Material> &map, MaterialMeta &meta) {
    auto currentMaterial = Material(meta);
    std::string currentMaterialName = meta.name;
    map.insert(std::make_pair(currentMaterialName, currentMaterial));
}

std::string OBJLoader::getMaterialFilePath(std::fstream &objectFile, const std::string &objectFilePath) {
    std::string materialFilePath;

    while(objectFile.good()) {
        std::string line;
        getline(objectFile, line);

        if(line.find("mtllib") == 0) {
            objectFile.close();
            materialFilePath = getMaterialPathFromLine(line, objectFilePath);
        }
    }

    return materialFilePath;
}

std::string OBJLoader::getMaterialPathFromLine(const std::string &currentLine, const std::string &objectFilePath) {
    std::string materialFileName = getMaterialFileNameFromLine(currentLine);
    std::string materialFilePath = getRelativePath(objectFilePath, materialFileName);
    return materialFilePath;
}

std::string OBJLoader::getMaterialFileNameFromLine(const std::string &line) {
    return parseObjString(line);
}

std::string OBJLoader::getMaterialName(const std::string &line) {
    return parseObjString(line);
}

std::string OBJLoader::parseObjString(const std::string &line) {
    std::vector<std::string> tokens;
    split(line, tokens, ' ');
    return tokens[1];
}

void OBJLoader::ParseFace(const std::string &line, std::vector<IObject> &coupled_indices) {
    std::vector<std::string> tokens;
    split(line, tokens, ' ');

    for(const auto& token: tokens) {
        std::vector<std::string> coupled_index_elements;
        split(token, coupled_index_elements, '/');
        IObject current{};
        current.m_position_index = std::stoi(coupled_index_elements[0]);
        current.m_texture_index = std::stoi(coupled_index_elements[1]);
        current.m_normal_index = std::stoi(coupled_index_elements[2]);

        coupled_indices.push_back(current);
    }
}

