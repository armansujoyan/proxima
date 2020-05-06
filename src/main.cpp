#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <helpers/RootDir.h>
#include <core/renderer/Shader.h>

#include <utils/Debug.h>

#include <core/renderer/Camera.h>
#include <core/loaders/OBJLoader.h>
#include <core/renderer/Mesh.h>
#include <core/physics/Collision.h>
#include <core/windowing/Window.h>
#include <core/renderer/Scene.h>
#include <core/renderer/Renderer.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(Window* window, float deltaTime);
void collideWithScene();

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const static std::string construction_path = ROOT_DIR "resources/objects/environments/construction/construction.obj";
const static std::string construction_collision_path = ROOT_DIR "resources/objects/environments/construction/collision_model/construction_collision.obj";

bool firstMouse = true;
float movementSpeed = 10.0f;
float lastX =  1920.0f / 2.0;
float lastY =  1080.0 / 2.0;

struct Ellipsoid {
    glm::vec3 radius;
    glm::vec3 position;
    glm::vec3 velocity;
};

Renderer* GLRenderer = new Renderer(glm::vec3(0.3f, 0.3f, 0.3f));
Camera* mainCamera = new Camera();
Scene* mainScene;

std::vector<Mesh*> construction;
std::vector<Mesh*> construction_collision;
std::vector<Triangle> sceneTriangles;
Ellipsoid character{};

void frameHandler(float deltaTime, Window* window) {
    processInput(window, deltaTime);
    GLRenderer->render(*mainScene);
};

int main()
{
    Window mainWindow = Window(SCR_WIDTH, SCR_HEIGHT, "Proxima | Demo");

    mainWindow.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mainWindow.setResizeCallback(framebuffer_size_callback);
    mainWindow.setMouseCallback(mouse_callback);
    mainWindow.setScrollCallback(scroll_callback);

    GLCall(glad_glEnable(GL_DEPTH_TEST));
//    GLCall(glad_glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ));

    character.radius = glm::vec3(0.2,0.2,0.2);
    character.velocity = glm::vec3(0, 0, 0);
    character.position = glm::vec3(0, 0, 0);

    construction = OBJLoader::load(construction_path);
    construction_collision = OBJLoader::load(construction_collision_path);

    mainCamera->setHeight(SCR_HEIGHT);
    mainCamera->setWidth(SCR_WIDTH);

    for(auto mesh: construction_collision) {
        sceneTriangles.insert(sceneTriangles.end(), mesh->getTriangles().begin(), mesh->getTriangles().end());
    }

    mainScene = new Scene(construction, mainCamera);

    GLRenderer->enableDepthTesting();

    mainWindow.onEachFrame(frameHandler);

    for(auto mesh: construction) {
        delete mesh;
    }

    for(auto mesh: construction_collision) {
        delete mesh;
    }

    delete mainScene;

    return 0;
}

void collideWithScene() {
    glm::vec3 nextPosition, integrationVelocity;
    bool collision = Collision::collideEllipsoid(character.position, character.radius, character.velocity,
                                sceneTriangles, nextPosition, integrationVelocity);
    int i = 0;
    while(collision && i < 5) {
        i++;
        collision = Collision::collideEllipsoid(nextPosition, character.radius, integrationVelocity,
                                                sceneTriangles, nextPosition, integrationVelocity);
    }
    mainCamera->Position = nextPosition;
    character.position = nextPosition;
    character.velocity = integrationVelocity;
}

void processInput(Window* window, float deltaTime)
{
    if (window->keyDown(GLFW_KEY_ESCAPE))
        window->close();

    float velocityFactor = movementSpeed * deltaTime;

    if (window->keyDown(GLFW_KEY_W)) {
        character.velocity = mainCamera->Front * velocityFactor;
        collideWithScene();
    }

    if (window->keyDown(GLFW_KEY_S)) {
        character.velocity = -mainCamera->Front * velocityFactor;
        collideWithScene();
    }

    if (window->keyDown(GLFW_KEY_A)) {
        character.velocity = -mainCamera->Right * velocityFactor;
        collideWithScene();
    }

    if (window->keyDown(GLFW_KEY_D)) {
        character.velocity = mainCamera->Right * velocityFactor;
        collideWithScene();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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