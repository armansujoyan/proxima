//
// Created by Arman Sujoyan on 4/15/20.
//

#ifndef PROXIMA_COLLISION_H
#define PROXIMA_COLLISION_H

#include <glm/vec3.hpp>

class Collision {
private:
    static bool isPointInsideTriangle(const glm::vec3 &point, const glm::vec3 &vertex1,
                               const glm::vec3 &vertex2, const glm::vec3 &vertex3, const glm::vec3 &triangleNormal);
};


#endif //PROXIMA_COLLISION_H
