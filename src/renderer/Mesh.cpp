//
// Created by Arman Sujoyan on 2/8/20.
//

#include "Mesh.h"

Mesh::Mesh(const std::string& path) {
    InitializeMesh(IndexedObject(path));
}

void Mesh::InitializeMesh(const IndexedObject& model) {
    m_IndexCount = model.m_indices.size();

    glGenVertexArrays(1, &m_VertexArrayID);
    glBindVertexArray(m_VertexArrayID);

    glGenBuffers(m_BufferCount, m_VertexBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[POSITION_BUFFER]);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_positions[0]) * model.m_indexed_positions.size(),
            &model.m_indexed_positions[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[TEXTURE_BUFFER]);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_textures[0]) * model.m_indexed_textures.size(),
            &model.m_indexed_textures[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers[NORMAL_BUFFER]);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(model.m_indexed_normals[0]) * model.m_indexed_normals.size(),
            &model.m_indexed_normals[0],
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexBuffers[INDEX_BUFFER]);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(model.m_indices[0]) * model.m_indices.size(),
            &model.m_indices[0],
            GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::Draw() {
    glBindVertexArray(m_VertexArrayID);

    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteBuffers(m_BufferCount, m_VertexBuffers);
    glDeleteVertexArrays(1, &m_VertexArrayID);
}