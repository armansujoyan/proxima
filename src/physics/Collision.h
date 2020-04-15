//
// Created by Arman Sujoyan on 4/15/20.
//

#ifndef PROXIMA_COLLISION_H
#define PROXIMA_COLLISION_H

#include <glm/vec3.hpp>

class Collision {
private:
    static bool sphereIntersectTriangle(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
            const glm::vec3 &vertex1, const glm::vec3 &vertex2, const glm::vec3 &vertex3,
            const glm::vec3 &triangleNormal, float &tMax, glm::vec3 &collisionNormal);
    static bool sphereIntersectPlane(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
            const glm::vec3 &planeNormal, const glm::vec3 &planePoint, float &tMax);
    static bool isPointInsideTriangle(const glm::vec3 &point, const glm::vec3 &vertex1,
            const glm::vec3 &vertex2, const glm::vec3 &vertex3, const glm::vec3 &triangleNormal);

    static bool
    sphereIntersectLineSegment(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                               const glm::vec3 &vertex1,
                               const glm::vec3 &vertex2, float &tMax, glm::vec3 &collisionNormal);
};


#endif //PROXIMA_COLLISION_H
