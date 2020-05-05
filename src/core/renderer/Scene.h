//
// Created by Arman Sujoyan on 5/5/20.
//

#ifndef PROXIMA_SCENE_H
#define PROXIMA_SCENE_H


#include "Mesh.h"
#include "Camera.h"

class Scene {
public:
    Scene(std::vector<Mesh*> meshes, Camera* camera);
    void render();
private:
    Camera* camera;
    std::vector<Mesh*> meshes;
};


#endif //PROXIMA_SCENE_H
