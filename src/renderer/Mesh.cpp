//
// Created by Arman Sujoyan on 2/8/20.
//

#include "Mesh.h"
#include "../utils/Debug.h"

Mesh::Mesh(const std::string& path) {
    InitializeMesh(IndexedObject(path));
}

void Mesh::InitializeMesh(const IndexedObject& model) {
    m_IndexCount = model.m_indices.size();

    GLCall(glGenVertexArrays(1, &m_VertexArrayID));
    GLCall(glBindVertexArray(m_VertexArrayID));

    GLCall(glGenBuffers(m_BufferCount, m_VertexBuffers));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[POSITION_BUFFER]));
    GLCall(glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_positions[0]) * model.m_indexed_positions.size(),
            &model.m_indexed_positions[0],
            GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[TEXTURE_BUFFER]));
    GLCall(glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_textures[0]) * model.m_indexed_textures.size(),
            &model.m_indexed_textures[0],
            GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[NORMAL_BUFFER]));
    GLCall(glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_normals[0]) * model.m_indexed_normals.size(),
            &model.m_indexed_normals[0],
            GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexBuffers[INDEX_BUFFER]));
    GLCall(glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(model.m_indices[0]) * model.m_indices.size(),
            &model.m_indices[0],
            GL_STATIC_DRAW));

    GLCall(glBindVertexArray(0));
}

void Mesh::Draw() {
    GLCall(glBindVertexArray(m_VertexArrayID));

    GLCall(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0));

    GLCall(glBindVertexArray(0));
}

Mesh::~Mesh() {
    GLCall(glDeleteBuffers(m_BufferCount, m_VertexBuffers));
    GLCall(glDeleteVertexArrays(1, &m_VertexArrayID));
}