//
// Created by Arman Sujoyan on 5/5/20.
//

#include <stdexcept>
#include "Renderer.h"

Renderer::Renderer(glm::vec3 clearColor) : clearColor(clearColor) {}

void Renderer::render(Scene &scene) {
    GLCall(glad_glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));
    GLCall(glad_glClear(clearMode));

    scene.render();
}

void Renderer::setClearColor(glm::vec3 color) {
    clearColor = color;
}

void Renderer::enableDepthTesting() {
    clearMode = clearMode | GL_DEPTH_BUFFER_BIT;
}

void Renderer::disableDepthTesting() {
    clearMode = GL_COLOR_BUFFER_BIT;
}
