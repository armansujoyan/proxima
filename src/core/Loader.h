//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_LOADER_H
#define PROXIMA_LOADER_H

#include <renderer/Mesh.h>

class Loader {
public:
    virtual Mesh load();
};

#endif //PROXIMA_LOADER_H
