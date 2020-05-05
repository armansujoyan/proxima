//
// Created by Arman Sujoyan on 5/5/20.
//

#ifndef PROXIMA_WINDOW_H
#define PROXIMA_WINDOW_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    void setInputMode(int mode, int value);
    void setResizeCallback(void (*fun)(GLFWwindow*, int, int));
    void setMouseCallback(void (*fun)(GLFWwindow*, double, double));
    void setScrollCallback(void (*fun)(GLFWwindow *, double, double));
    bool isOpen();

    bool keyDown(int i);

    void close();

    void updateFram();

private:
    GLFWwindow* window;
    unsigned int width;
    unsigned int height;
    std::string title;
    float deltaTime;
    float lastFrame;
};


#endif //PROXIMA_WINDOW_H