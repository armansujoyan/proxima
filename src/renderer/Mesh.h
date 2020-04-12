//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_MESH_H
#define PROXIMA_MESH_H

#include <glad/glad.h>
#include <string>
#include <map>

#include "IndexedGeometry.h"
#include "Material.h"

enum MeshBufferPosition {
    POSITION_BUFFER,
    TEXTURE_BUFFER,
    NORMAL_BUFFER,
    INDEX_BUFFER
};

class Mesh {
public:
    explicit Mesh(const IndexedGeometry *indexedObject);
    Mesh(const IndexedGeometry *geometry, Material materialsMap);

    void Draw();

    virtual ~Mesh();
    Material m_material;
private:

    static const unsigned int m_BufferCount = 4;

    void InitializeMesh(const IndexedGeometry *obj);
    GLuint m_VertexArrayID{};
    GLuint m_VertexBuffers[m_BufferCount]{};
    unsigned int m_IndexCount{};
};


#endif //PROXIMA_MESH_H
