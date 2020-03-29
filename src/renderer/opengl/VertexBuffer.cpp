//
// Created by Arman Sujoyan on 3/29/20.
//

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() {}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glad_glGenBuffers(1, &m_vboId));
    bind();
    GLCall(glad_glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glad_glDeleteBuffers(1, &m_vboId));
}

void VertexBuffer::bind() const{
    GLCall(glad_glBindBuffer(GL_ARRAY_BUFFER, m_vboId));
}

void VertexBuffer::unbind() const {
    GLCall(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
}
