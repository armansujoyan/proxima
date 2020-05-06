//
// Created by Arman Sujoyan on 5/5/20.
//

#include "Window.h"

Window::Window(unsigned int width, unsigned int height, const std::string& title) : width{width}, height{height},
    title{title}  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::setInputMode(int mode, int value) {
    glfwSetInputMode(window, mode, value);
}

void Window::setResizeCallback(void (*fun)(GLFWwindow*, int, int)) {
    glfwSetFramebufferSizeCallback(window, fun);
}

void Window::setMouseCallback(void (*fun)(GLFWwindow*, double, double)) {
    glfwSetCursorPosCallback(window, fun);
}

void Window::setScrollCallback(void (*fun)(GLFWwindow *, double, double)) {
    glfwSetScrollCallback(window, fun);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(window);
}

bool Window::keyDown(int i) {
    return glfwGetKey(window, i) == GLFW_PRESS;
}

void Window::close() {
    glfwSetWindowShouldClose(window, true);
}

void Window::updateFrame() {
    glfwSwapBuffers(window);
}

void Window::onEachFrame(void (*frameHandler)(float, Window*)) {
    if (isFrameCallbackSet) {
        throw std::runtime_error("Frame function is already set in window.");
    }

    isFrameCallbackSet = true;

    while(isOpen()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameHandler(deltaTime, this);

        updateFrame();
        glfwPollEvents();
    }
}

Window::~Window() {
    glfwTerminate();
}


