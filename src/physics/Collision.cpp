//
// Created by Arman Sujoyan on 4/15/20.
//

#include <glm/glm.hpp>
#include <renderer/IndexedGeometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Collision.h"

bool Collision::sphereIntersectTriangle(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                                        const glm::vec3 &vertex1, const glm::vec3 &vertex2, const glm::vec3 &vertex3,
                                        const glm::vec3 &triangleNormal, float &tMax, glm::vec3 &collisionNormal) {
    float t = tMax;
    glm::vec3 collisionCenter;
    bool edgeCollided = false;

    if( !sphereIntersectPlane(center, radius, velocity, triangleNormal, vertex1, t)) return false;

    if(t < 0) {
        collisionCenter = center - (t * triangleNormal);
    } else {
        collisionCenter = center + (t * velocity);
    }

    if ( isPointInsideTriangle(collisionCenter, vertex1, vertex2, vertex3, triangleNormal)) {
        collisionNormal = triangleNormal;
        tMax = t;
        return true;
    }

    edgeCollided |= sphereIntersectLineSegment(center, radius, velocity, vertex1, vertex2, tMax, collisionNormal);

    edgeCollided |= sphereIntersectLineSegment(center, radius, velocity, vertex2, vertex3, tMax, collisionNormal);

    edgeCollided |= sphereIntersectLineSegment(center, radius, velocity, vertex3, vertex1, tMax, collisionNormal);

    return edgeCollided;
}

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
    float numerator = glm::dot(center - planePoint, planeNormal) - radius;
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

bool Collision::sphereIntersectLineSegment(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                                           const glm::vec3 &vertex1, const glm::vec3 &vertex2, float &tMax,
                                           glm::vec3 &collisionNormal) {
    glm::vec3 edge, centerToVertex, pointOnEdge, collisionCenter;
    float t;

    edge = vertex2 - vertex1;
    centerToVertex = center - vertex1;

    float edgeLength = glm::length(edge);

    if (edgeLength < 1e-5f) return false;

    edge /= edgeLength;

    glm::vec3 centerToVertexEdgeCross = glm::cross(centerToVertex, edge);
    glm::vec3 velocityEdgeCross = glm::cross(velocity, edge);

    float velocityEdgeCrossLength = glm::length(velocityEdgeCross);
    float centerToVertDotEdgeCross = glm::dot(centerToVertexEdgeCross, velocityEdgeCross);

    float a = velocityEdgeCrossLength * velocityEdgeCrossLength;
    float b = 2.0f * glm::dot(velocityEdgeCross, centerToVertexEdgeCross);
    float c = centerToVertDotEdgeCross * centerToVertDotEdgeCross - (radius * radius);

    if ( c < 0.0f) {
        float centerProjectionOnLine = glm::dot(centerToVertex, edge);

        if (centerProjectionOnLine < 0.0f) {
            return sphereIntersectPoint(center, radius, velocity, vertex1, tMax, collisionNormal);
        } else if (centerProjectionOnLine > edgeLength) {
            return sphereIntersectPoint(center, radius, velocity, vertex2, tMax, collisionNormal);
        } else {
            pointOnEdge = vertex1 + (edge * centerProjectionOnLine);
            collisionNormal = glm::normalize(center - pointOnEdge);
            float collisionNormalLength = glm::length(collisionNormal);

            t = collisionNormalLength - radius;
            if (tMax < t) return false;

            tMax = t;

            return true;
        }
    }

    if (tMax < 0.0f) return false;

    if ( !solveCollision(a, b, c, t)) return false;

    if ( t > tMax) return false;

    collisionCenter = center  + (t * velocity);

    float collisionCenterProjectionOnLine = glm::dot(collisionCenter - vertex1, edge);

    if (collisionCenterProjectionOnLine < 0.0f) {
        return sphereIntersectPoint(center, radius, velocity, vertex1, tMax, collisionNormal);
    } else if (collisionCenterProjectionOnLine > edgeLength) {
        return sphereIntersectPoint(center, radius, velocity, vertex2, tMax, collisionNormal);
    }

    pointOnEdge = vertex1 + (edge * t);
    collisionNormal = glm::normalize(collisionCenter - pointOnEdge);
    tMax = t;

    return true;
}

bool Collision::sphereIntersectPoint(const glm::vec3 &center, float radius, const glm::vec3 &velocity,
                                     const glm::vec3 &point, float &tMax, glm::vec3 &collisionNormal) {
    float t;

    glm::vec3 pointToCenter = center - point;
    float velocityLength = glm::length(velocity);
    float pointToCenterLength = glm::length(pointToCenter);

    float a = velocityLength * velocityLength;
    float b = 2.0f * glm::dot(velocity, pointToCenter);
    float c = (pointToCenterLength * pointToCenterLength) - (radius * radius);

    if (c < 0.0f) {
        t = pointToCenterLength - radius;

        if(tMax < t) return false;

        collisionNormal = glm::normalize(pointToCenter);

        tMax = t;

        return true;
    }

    if (tMax < 0.0f) return false;

    if ( !solveCollision(a, b, c, t)) return false;

    if (t > tMax) return false;

    glm::vec3 collisionCenter = center + (velocity * t);

    collisionNormal = glm::normalize(collisionCenter - point);
    tMax = t;

    return false;
}

bool Collision::solveCollision(float a, float b, float c, float &t) {
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0.0f) return false;

    float sqrtDiscriminant = sqrtf(discriminant);

    float t0 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t1 = (-b + sqrtDiscriminant) / (2.0f * a);

    float temp;
    if (t1 < t0) { temp = t0; t0 = t1; t1 = temp; }

    if (t1 < 0.0f) return false;

    if (t0 < 0.0f) t = t1; else t = t0;

    return true;
}

bool Collision::ellipsoidIntersectScene(const glm::vec3 &center, const glm::vec3 &radius, const glm::vec3 &velocity,
                                        Intersection *intersections, unsigned int &intersectionCount,
                                        const std::vector<Triangle> &triangles) {
    glm::vec3 eSpaceCenter, eSpaceVelocity, InverseRadius;
    glm::vec3 ellipsoidVertices[3], eNormal;
    glm::vec3 eIntersectNormal, eNewCenter;
    unsigned int counter, newIndex = 0, fistIndex;
    bool addToList;
    float eInterval;
    unsigned int i;

    InverseRadius = glm::vec3(1 / radius.x, 1 / radius.y, 1 / radius.z);
    glm::mat4 ellipsoidSpaceMatrix = glm::scale(glm::mat4(1.0f), InverseRadius);
    glm::mat4 ellipsoidSpaceRadiusMatrix = glm::scale(glm::mat4( 1.0f), radius);

    eSpaceCenter = center;
    eSpaceVelocity = velocity;

    eInterval = 1.0f;
    intersectionCount = 0;

    auto triangle_iterator = triangles.begin();
    for(counter = 0; triangle_iterator != triangles.end(); ++triangle_iterator, ++counter) {
        ellipsoidVertices[0] = glm::vec4(triangle_iterator->vertex1, 1.0f) * ellipsoidSpaceMatrix;
        ellipsoidVertices[1] = glm::vec4(triangle_iterator->vertex2, 1.0f) * ellipsoidSpaceMatrix;
        ellipsoidVertices[2] = glm::vec4(triangle_iterator->vertex3, 1.0f) * ellipsoidSpaceMatrix;

        eNormal = glm::vec4(triangle_iterator->normal, 1.0f) * ellipsoidSpaceRadiusMatrix;
        eNormal = glm::normalize(eNormal);

         if (sphereIntersectTriangle(eSpaceCenter, 1.0f, eSpaceVelocity, ellipsoidVertices[0],
                 ellipsoidVertices[1],ellipsoidVertices[2], eNormal,eInterval,
                 eIntersectNormal)) {
             if (eInterval > 0) {
                 eNewCenter = eSpaceCenter + (eInterval * eSpaceVelocity);
             } else {
                 eNewCenter = eSpaceCenter - (eIntersectNormal * eSpaceVelocity);
             }

             addToList = false;
             if (intersectionCount == 0 || eInterval < intersections[0].t) {
                 addToList = true;
                 newIndex = 0;
                 intersectionCount = 1;
             } else if(fabs(eInterval < intersections[0].t) < 1e-5f) {
                 if (intersectionCount < m_maxIntersections) {
                     addToList = true;
                     newIndex = intersectionCount;
                     intersectionCount++;
                 }
             }

             if(addToList) {
                 intersections[newIndex].t = eInterval;
                 intersections[newIndex].intersectionNormal = eIntersectNormal;
                 intersections[newIndex].nextCenter = eNewCenter + (eIntersectNormal * 1e-3f);
                 intersections[newIndex].intersectionPoint = eNewCenter - eIntersectNormal;
                 intersections[newIndex].triangleIndex = counter;
             }
         }
    }

    return (intersectionCount > 0);
}
