//
// Created by Arman Sujoyan on 2/8/20.
//

#include "Mesh.h"
#include "Material.h"
#include <utils/Debug.h>

#include <utility>

Mesh::Mesh(const IndexedObject *indexedObject) {
    InitializeMesh(indexedObject);
}

Mesh::Mesh(const IndexedObject *geometry,std::map<std::string, Material> materialsMap):
    m_material_map(std::move(materialsMap)) {
    InitializeMesh(geometry);
}

void Mesh::InitializeMesh(const IndexedObject *model) {
    m_IndexCount = model->m_indices.size();

    GLCall(glad_glGenVertexArrays(1, &m_VertexArrayID));
    GLCall(glad_glBindVertexArray(m_VertexArrayID));

    GLCall(glad_glGenBuffers(m_BufferCount, m_VertexBuffers));

    GLCall(glad_glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[POSITION_BUFFER]));
    GLCall(glad_glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model->m_indexed_positions[0]) * model->m_indexed_positions.size(),
            &model->m_indexed_positions[0],
            GL_STATIC_DRAW));
    GLCall(glad_glEnableVertexAttribArray(0));
    GLCall(glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, nullptr));

    GLCall(glad_glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[TEXTURE_BUFFER]));
    GLCall(glad_glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model->m_indexed_textures[0]) * model->m_indexed_textures.size(),
            &model->m_indexed_textures[0],
            GL_STATIC_DRAW));
    GLCall(glad_glEnableVertexAttribArray(1));
    GLCall(glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr));

    GLCall(glad_glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[NORMAL_BUFFER]));
    GLCall(glad_glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model->m_indexed_normals[0]) * model->m_indexed_normals.size(),
            &model->m_indexed_normals[0],
            GL_STATIC_DRAW));
    GLCall(glad_glEnableVertexAttribArray(2));
    GLCall(glad_glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    GLCall(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexBuffers[INDEX_BUFFER]));
    GLCall(glad_glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(model->m_indices[0]) * model->m_indices.size(),
            &model->m_indices[0],
            GL_STATIC_DRAW));

    GLCall(glad_glBindVertexArray(0));
}

void Mesh::Draw() {
    //m_material_map.find("car9:Car_18C_D")->second.bindDiffuse();

    GLCall(glad_glBindVertexArray(m_VertexArrayID));

    GLCall(glad_glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr));

    GLCall(glad_glBindVertexArray(0));
}

Mesh::~Mesh() {
    GLCall(glad_glDeleteBuffers(m_BufferCount, m_VertexBuffers));
    GLCall(glad_glDeleteVertexArrays(1, &m_VertexArrayID));
}
