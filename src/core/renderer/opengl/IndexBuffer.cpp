//
// Created by Arman Sujoyan on 3/29/20.
//

#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() {}

IndexBuffer::IndexBuffer(const void* data, unsigned int count) :
        m_indexCount(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glad_glGenBuffers(1, &m_eboId));
    GLCall(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId));
    GLCall(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glad_glDeleteBuffers(1, &m_eboId));
}

void IndexBuffer::bind() const {
    GLCall(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId));
}

void IndexBuffer::unbind() const {
    GLCall(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
