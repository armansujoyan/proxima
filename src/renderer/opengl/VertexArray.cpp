//
// Created by Arman Sujoyan on 3/29/20.
//

#include "VertexArray.h"

VertexArray::VertexArray() {
    GLCall(glad_glGenVertexArrays(1, &m_vaoId));
}

VertexArray::~VertexArray() {
    GLCall(glad_glDeleteVertexArrays(1, &m_vaoId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
    this->bind();
    vb.bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements->size(); i++){
        const auto& element = elements->at(i);
        GLCall(glEnableVertexAttribArray(i))
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                     layout.GetStride(), (const void*)(size_t)offset))
        offset += element.count * VAOElement::GetSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GLCall(glad_glBindVertexArray(m_vaoId));
}

void VertexArray::unbind() const {
    GLCall(glad_glBindVertexArray(0));
}

void VertexArray::AddBuffer(const IndexBuffer &ib) {
    this->bind();
    ib.bind();
    this->unbind();
}

void VertexArray::AddBuffer(const std::vector<VertexBuffer*>& vertexBuffers, VertexBufferLayout* layout) {
    this->bind();
    const auto& elements = layout->GetElements();

    for(size_t i = 0; i < vertexBuffers.size(); i++) {
        vertexBuffers[i]->bind();
        auto element = elements->at(i);
        GLCall(glEnableVertexAttribArray(i))
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, 0, nullptr))
    }

    this->unbind();
}
