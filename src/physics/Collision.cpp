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

bool Collision::sphereIntersectPlane(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                                     const glm::vec3 &planeNormal, const glm::vec3 &planePoint, float &tMax) {
    float numerator = glm::dot(center - planePoint, planeNormal);
    float denominator = glm::dot(velocity, planeNormal);

    if (numerator < 0.0f || denominator > -0.000001f) {

        if (denominator > -1e-5f) return false;

        if (numerator < -radius) return false;

        tMax = numerator;

        return true;
    }

    float t = -(numerator/denominator);

    if (t < 0.0f || t > tMax) return false;

    tMax = t;

    return true;
}
