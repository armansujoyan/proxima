//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_MESH_H
#define PROXIMA_MESH_H

#include <glad/glad.h>
#include <string>
#include <map>

#include "IndexedObject.h"
#include "Material.h"

enum MeshBufferPosition {
    POSITION_BUFFER,
    TEXTURE_BUFFER,
    NORMAL_BUFFER,
    INDEX_BUFFER
};

class Mesh {
public:
    explicit Mesh(const IndexedObject *indexedObject);
    Mesh(const IndexedObject *geometry,std::map<std::string, Material> materialsMap);

    void Draw();

    virtual ~Mesh();
    std::map<std::string, Material> m_material_map;
private:

    static const unsigned int m_BufferCount = 4;

    void InitializeMesh(const IndexedObject *obj);
    GLuint m_VertexArrayID{};
    GLuint m_VertexBuffers[m_BufferCount]{};
    unsigned int m_IndexCount{};
};


#endif //PROXIMA_MESH_H
