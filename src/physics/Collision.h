//
// Created by Arman Sujoyan on 4/15/20.
//

#ifndef PROXIMA_COLLISION_H
#define PROXIMA_COLLISION_H

#include <glm/vec3.hpp>
#include <renderer/IndexedGeometry.h>
#include <vector>

struct Intersection {
    glm::vec3 nextCenter;
    glm::vec3 intersectionPoint;
    glm::vec3 intersectionNormal;
    float t;
    unsigned int triangleIndex;
};

class Collision {
    static bool
    collideEllipsoid(const glm::vec3 &center, const glm::vec3 &radius, const glm::vec3 &velocity,
                     const std::vector<Triangle> &triangles, glm::vec3 &newCenter, glm::vec3 &integrationVelocity);
private:
    const static unsigned int m_maxIntersections = 10;
    static bool ellipsoidIntersectScene(const glm::vec3 &center, const glm::vec3 &radius, const glm::vec3 &velocity,
                                        Intersection *intersections, unsigned int &intersectionCount,
                                        const std::vector<Triangle> &triangles);
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

    static bool
    sphereIntersectPoint(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                         const glm::vec3 &point, float &tMax, glm::vec3 &collisionNormal);

    static bool solveCollision(float a, float b, float c, float &t);
};


#endif //PROXIMA_COLLISION_H
