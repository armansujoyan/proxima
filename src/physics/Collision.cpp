//
// Created by Arman Sujoyan on 4/15/20.
//

#include <glm/glm.hpp>
#include "Collision.h"

bool Collision::isPointInsideTriangle(const glm::vec3 &point, const glm::vec3 &vertex1, const glm::vec3 &vertex2,
                                      const glm::vec3 &vertex3, const glm::vec3 &triangleNormal) {
    glm::vec3 edge, edgeNormal, pointToEdge;

    edge = vertex2 - vertex1;
    pointToEdge = vertex1 - point;
    edgeNormal = glm::cross(edge, triangleNormal);

    if (glm::dot(pointToEdge, triangleNormal) < 0.0f) return false;

    edge = vertex3 - vertex2;
    pointToEdge = vertex2 - point;
    edgeNormal = glm::cross(edge, triangleNormal);

    if (glm::dot(pointToEdge, triangleNormal) < 0.0f) return false;

    edge = vertex1 - vertex3;
    pointToEdge = vertex3 - point;
    edgeNormal = glm::cross(edge, triangleNormal);

    return !(glm::dot(pointToEdge, triangleNormal) < 0.0f);

}
