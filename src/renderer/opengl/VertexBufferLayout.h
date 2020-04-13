//
// Created by Arman Sujoyan on 3/29/20.
//

#ifndef PROXIMA_VERTEXBUFFERLAYOUT_H
#define PROXIMA_VERTEXBUFFERLAYOUT_H

#include <vector>
#include "utils/Debug.h"

struct VAOElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type){
        switch(type){
            case GL_FLOAT:
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    };
};

class VertexBufferLayout {
private:
    std::vector<VAOElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
            : m_Stride(0) {};

    template<typename T> inline static constexpr bool false_v = false;

    template<typename T>
    void Push(unsigned int count, bool normalized){
        static_assert(false_v<T>);
    };

    template<>
    void Push<float>(unsigned int count, bool normalized){
        m_Elements.push_back({ GL_FLOAT, count, normalized });
        m_Stride += count * VAOElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count, bool normalized){
        m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_Stride += count * VAOElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count, bool normalized){
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_Stride += count * VAOElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VAOElement>* GetElements() const { return &m_Elements; };
    inline unsigned int GetStride() const { return m_Stride; };

};

#endif //PROXIMA_VERTEXBUFFERLAYOUT_H
