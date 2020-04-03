//
// Created by Arman Sujoyan on 4/3/20.
//

#include "OBJLoader.h"

Mesh OBJLoader::load(const std::string &path) {
    return Mesh(IndexedObject(path));
}