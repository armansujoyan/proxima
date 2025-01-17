//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_OBJLOADER_H
#define PROXIMA_OBJLOADER_H

#include "map"
#include <core/loaders/Loader.h>
#include <core/renderer/Mesh.h>
#include <core/renderer/Material.h>

typedef std::pair<std::string, IndexedGeometry*> GeometryMaterialPair;

class OBJLoader: public Loader {
public:
    static std::vector<Mesh*> load(const std::string& path);
private:
    static std::map<std::string, MaterialMeta> parseMaterials(const std::string& path);

    static std::map<std::string, MaterialMeta>
    getMaterialsMetaMap(const std::string &materialFilePath, const std::string &objectFilePath);

    static std::string getMaterialPathFromObjectFile(const std::string &basicString);
    static std::string getMaterialFilePath(std::fstream &objectFile, const std::string &objectFilePath);
    static std::string getMaterialPathFromLine(const std::string &currentLine, const std::string &objectFilePath);
    static std::string getMaterialFileNameFromLine(const std::string &line);

    static std::string getMaterialName(const std::string &line);
    static std::string parseObjString(const std::string &line);

    static std::vector<GeometryMaterialPair> parseGeometry(const std::string &path);
    static void ParseFace(const std::string& line, std::vector<IObject> &couples_indices);

    static IObject getParsedIndicesFromIndexSet(const std::string &faceString);

    static void parseFaceTokenAndAddToCoupledIndices(const std::string &faceToken, std::vector<IObject> &coupledIndices);
};


#endif //PROXIMA_OBJLOADER_H
