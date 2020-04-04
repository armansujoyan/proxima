//
// Created by Arman Sujoyan on 4/3/20.
//

#include <renderer/Material.h>
#include <utils/StringUtils.h>
#include <utils/Logging.h>
#include "OBJLoader.h"

#include <map>

Mesh OBJLoader::load(const std::string &path) {
    IndexedObject geometry = IndexedObject(path);
    std::map<std::string, Material> materialsMap = parseMaterials(path);
    return Mesh(geometry);
}

std::map<std::string, Material> OBJLoader::parseMaterials(const std::string &objectPath) {
    std::string mtlPath = getMaterialPathFromObjectFile(objectPath);
    std::map<std::string, Material> materialsMap = getMaterialsMap(mtlPath, objectPath);

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
    Material currentMaterial = Material(meta);
    std::string currentMaterialName = meta.name;
    std::pair<std::string, Material> nameToMaterial = std::pair(currentMaterialName, currentMaterial);
    map.insert(nameToMaterial);
}

std::string OBJLoader::getMaterialFilePath(std::fstream &objectFile, const std::string &objectFilePath) {
    while(objectFile.good()) {
        std::string line;
        getline(objectFile, line);

        if(line.find("mtllib") != std::string::npos) {
            objectFile.close();
            return getMaterialPathFromLine(line, objectFilePath);
        }
    }
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
