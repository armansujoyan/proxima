//
// Created by Arman Sujoyan on 5/5/20.
//

#include "Scene.h"

Scene::Scene(std::vector<Mesh *> meshes, Camera *camera) : meshes(meshes), camera(camera) {}

void Scene::render() {
    for(Mesh* mesh: meshes) {
        Shader* shader = mesh->getShader();
        shader->use();
        mesh->Draw();

        glm::mat4 projection = camera->GetProjectionMatrix();
        shader->setMat4("projection", projection);

        glm::mat4 view = camera->GetViewMatrix();
        shader->setMat4("view", view);
    }
}

