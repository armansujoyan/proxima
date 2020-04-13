//
// Created by Arman Sujoyan on 2/8/20.
//

#ifndef PROXIMA_MESH_H
#define PROXIMA_MESH_H

#include <glad/glad.h>
#include <string>
#include <map>
#include <renderer/opengl/VertexArray.h>

#include "IndexedGeometry.h"
#include "Material.h"

class Mesh {
public:
    explicit Mesh(const IndexedGeometry *indexedObject);
    Mesh(const IndexedGeometry *geometry, Material* material);
    ~Mesh();

    void Draw();
    Material* m_material;
private:
    void InitializeMesh(const IndexedGeometry *obj);
    VertexArray* meshVao;
    std::vector<VertexBuffer*> meshVertexBuffers;
    IndexBuffer* meshIndexBuffer;
    unsigned int m_IndexCount{};
};


#endif //PROXIMA_MESH_H
