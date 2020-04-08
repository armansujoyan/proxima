//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_OBJLOADER_H
#define PROXIMA_OBJLOADER_H

#include "map"
#include <core/Loader.h>
#include <renderer/Mesh.h>
#include <renderer/Material.h>

class OBJLoader: public Loader {
public:
    static Mesh * load(const std::string& path);
private:
    static std::map<std::string, Material> parseMaterials(const std::string& path);

    static std::map<std::string, Material>
    getMaterialsMap(const std::string &materialFilePath, const std::string &objectFilePath);
    static void addMaterialFromMetaToMap(std::map<std::string, Material> &map, MaterialMeta &meta);

    static std::string getMaterialPathFromObjectFile(const std::string &basicString);
    static std::string getMaterialFilePath(std::fstream &objectFile, const std::string &objectFilePath);
    static std::string getMaterialPathFromLine(const std::string &currentLine, const std::string &objectFilePath);
    static std::string getMaterialFileNameFromLine(const std::string &line);

    static std::string getMaterialName(const std::string &line);
    static std::string parseObjString(const std::string &line);
};


#endif //PROXIMA_OBJLOADER_H
