//
// Created by Arman Sujoyan on 3/29/20.
//

#ifndef PROXIMA_VERTEXARRAY_H
#define PROXIMA_VERTEXARRAY_H

#include "utils/Debug.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddBuffer(const std::vector<VertexBuffer*>& vertexBuffers, VertexBufferLayout* layout);
    void AddBuffer(const IndexBuffer& ib);
private:
    unsigned int m_vaoId;
};

#endif //PROXIMA_VERTEXARRAY_H
