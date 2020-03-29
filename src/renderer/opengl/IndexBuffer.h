//
// Created by Arman Sujoyan on 3/29/20.
//

#ifndef PROXIMA_INDEXBUFFER_H
#define PROXIMA_INDEXBUFFER_H

#include "utils/Debug.h"

class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
private:
    unsigned int m_eboId;
    unsigned int m_indexCount;
};

#endif //PROXIMA_INDEXBUFFER_H
