//
//  Debug.h
//  proxima_engine
//
//  Created by Arman Sujoyan on 10/31/19.
//  Copyright © 2019 Arman Sujoyan. All rights reserved.
//

#ifndef Debug_h
#define Debug_h

#include <glad/glad.h>

#include <iostream>

#define ASSERT(x) if (!(x)) __asm__("int $3");

void inline GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool inline GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()){
        std::cout << "[OpenGL Error] (" << std::hex << error << "): " << function <<
                  " " << file << ":" << std::dec << line << std::endl;
        return false;
    }
    return true;
}

#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#endif /* Debug_h */
