//
// Created by Arman Sujoyan on 2/8/20.
//

#include "Mesh.h"
#include "Material.h"
#include <utils/Debug.h>
#include <renderer/opengl/VertexArray.h>

Mesh::Mesh(const IndexedGeometry *indexedObject) {
    InitializeMesh(indexedObject);
}

Mesh::Mesh(const IndexedGeometry *geometry, Material* material):
    m_material(material) {
    InitializeMesh(geometry);
}

void Mesh::InitializeMesh(const IndexedGeometry *model) {
    m_IndexCount = model->m_indices.size();

    meshVao = new VertexArray();
    auto *vLayout = new VertexBufferLayout();
    vLayout->Push<float>(3, false);
    vLayout->Push<float>(2, false);
    vLayout->Push<float>(3, false);

    auto *positionBuffer = new VertexBuffer(&model->m_indexed_positions[0],
            sizeof(model->m_indexed_positions[0]) * model->m_indexed_positions.size());
    meshVertexBuffers.push_back(positionBuffer);

    auto *textureBuffer = new VertexBuffer(&model->m_indexed_textures[0],
            sizeof(model->m_indexed_textures[0]) * model->m_indexed_textures.size());
    meshVertexBuffers.push_back(textureBuffer);

    auto *normalBuffer = new VertexBuffer(&model->m_indexed_normals[0],
            sizeof(model->m_indexed_normals[0]) * model->m_indexed_normals.size());
    meshVertexBuffers.push_back(normalBuffer);

    meshIndexBuffer = new IndexBuffer(&model->m_indices[0], model->m_indices.size());

    meshVao->AddBuffer(meshVertexBuffers, vLayout);
    meshVao->AddBuffer(*meshIndexBuffer);
}

void Mesh::Draw() {
    m_material->attach();

    meshVao->bind();

    GLCall(glad_glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));

    meshVao->unbind();

    m_material->detach();
}

Mesh::~Mesh() {
    delete meshVao;
    delete meshIndexBuffer;
    delete m_material;

    for(auto buffer: meshVertexBuffers) {
        delete buffer;
    }
}
