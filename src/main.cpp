#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <helpers/RootDir.h>
#include <renderer/Shader.h>

#include <utils/Debug.h>

#include <iostream>
#include <renderer/Camera.h>
#include <core/OBJLoader.h>
#include <renderer/Mesh.h>
#include <physics/Collision.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void collideWithScene();

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const static std::string construction_path = ROOT_DIR "resources/objects/environments/construction/collision_model/construction_collision.obj";

bool firstMouse = true;
float movementSpeed = 10.0f;
float lastX =  1920.0f / 2.0;
float lastY =  1080.0 / 2.0;

struct Ellipsoid {
    glm::vec3 radius;
    glm::vec3 position;
    glm::vec3 velocity;
};

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera* mainCamera = new Camera;
std::vector<Mesh*> construction;
std::vector<Triangle> sceneTriangles;
Ellipsoid character{};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    mainCamera->setMovementSpeed(10);
    Shader ourShader(ROOT_DIR "resources/shaders/default.vs", ROOT_DIR "resources/shaders/default.fs");
    ourShader.use();

    GLCall(glad_glEnable(GL_DEPTH_TEST));
    GLCall(glad_glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ));

    character.radius = glm::vec3(0.3,0.3,0.3);
    character.velocity = glm::vec3(0, 0, 0);
    character.position = glm::vec3(0, 0, 0);

    construction = OBJLoader::load(construction_path);

    for(auto mesh: construction) {
        sceneTriangles.insert(sceneTriangles.end(), mesh->getTriangles().begin(), mesh->getTriangles().end());
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        GLCall(glad_glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GLCall(glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        ourShader.use();
        for(auto mesh: construction) {
            mesh->Draw();
        }

        glm::mat4 projection = glm::perspective(glm::radians(mainCamera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = mainCamera->GetViewMatrix();
        ourShader.setMat4("view", view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Remove allocated memory from the heap
    for(auto mesh: construction) {
        delete mesh;
    }

    delete mainCamera;
    // Remove allocated memory from heap

    glfwTerminate();
    return 0;
}

void collideWithScene() {
    glm::vec3 nextPosition, integrationVelocity;
    Collision::collideEllipsoid(character.position, character.radius, character.velocity,
                                sceneTriangles, nextPosition, integrationVelocity);
    mainCamera->Position = nextPosition;
    character.position = nextPosition;
    character.velocity = integrationVelocity;
    std::cout << glm::to_string(character.position) << std::endl;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float velocityFactor = movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//        mainCamera->ProcessKeyboard(FORWARD, deltaTime);
        character.velocity = mainCamera->Front * velocityFactor;
        collideWithScene();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mainCamera->ProcessKeyboard(BACKWARD, deltaTime);
        character.position = mainCamera->Position;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mainCamera->ProcessKeyboard(LEFT, deltaTime);
        character.position = mainCamera->Position;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mainCamera->ProcessKeyboard(RIGHT, deltaTime);
        character.position = mainCamera->Position;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    mainCamera->ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mainCamera->ProcessMouseScroll(yoffset);
}