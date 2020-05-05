//
// Created by Arman Sujoyan on 3/29/20.
//

#ifndef PROXIMA_VERTEXBUFFER_H
#define PROXIMA_VERTEXBUFFER_H

#include "utils/Debug.h"

class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
private:
    unsigned int m_vboId;
};


#endif //PROXIMA_VERTEXBUFFER_H
