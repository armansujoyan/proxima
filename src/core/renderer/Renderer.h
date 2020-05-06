//
// Created by Arman Sujoyan on 5/5/20.
//

#ifndef PROXIMA_RENDERER_H
#define PROXIMA_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"


class Renderer {
public:
    explicit Renderer(glm::vec3 clearColor);
    void render(Scene& scene);
    void setClearColor(glm::vec3 clearColor);
    void enableDepthTesting();
    void disableDepthTesting();
private:
    glm::vec3 clearColor;
    GLenum clearMode = GL_COLOR_BUFFER_BIT;
};


#endif //PROXIMA_RENDERER_H
