//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_OBJLOADER_H
#define PROXIMA_OBJLOADER_H

#include <core/Loader.h>
#include <renderer/Mesh.h>

class OBJLoader: public Loader {
public:
    static Mesh load(const std::string& path);
};


#endif //PROXIMA_OBJLOADER_H
