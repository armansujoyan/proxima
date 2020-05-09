//
// Created by Arman Sujoyan on 4/3/20.
//

#ifndef PROXIMA_LOADER_H
#define PROXIMA_LOADER_H

#include <core/renderer/Mesh.h>

class Loader {
public:
    virtual std::vector<Mesh*> load();
};

#endif //PROXIMA_LOADER_H
