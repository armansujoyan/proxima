//
// Created by Arman Sujoyan on 2/5/20.
//

#ifndef PROXIMA_SHADER_H
#define PROXIMA_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>

#include "utils/Debug.h"

class Shader
{
public:
    unsigned int ID;

    Shader(): ID(0),
    vertexShaderPath(""), fragmentShaderPath(""), geometryShaderPath("") {};

    Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "") :
        ID(0),
        vertexShaderPath(std::move(vertexPath)),
        fragmentShaderPath(std::move(fragmentPath)),
        geometryShaderPath(std::move(std::move(geometryPath)))
    {
        initializeShader(
                vertexShaderPath.c_str(),
                fragmentShaderPath.c_str(),
                geometryShaderPath.c_str());
    }

    Shader(const Shader &S): Shader(S.vertexShaderPath, S.fragmentShaderPath, S.geometryShaderPath) {}

    Shader & operator=(const Shader &S) {
        if (this == &S) {
            return *this;
        }
        else {
            GLCall(glad_glDeleteProgram(ID));

            initializeShader(
                    S.vertexShaderPath.c_str(),
                    S.fragmentShaderPath.c_str(),
                    S.geometryShaderPath.c_str());

            return *this;
        }
    }

    ~Shader() {
        GLCall(glad_glDeleteProgram(ID));
    }

    void initializeShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if(geometryPath[0] != '\0')
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        GLCall(glShaderSource(vertex, 1, &vShaderCode, nullptr));
        GLCall(glCompileShader(vertex));
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        GLCall(glShaderSource(fragment, 1, &fShaderCode, nullptr));
        GLCall(glCompileShader(fragment));
        checkCompileErrors(fragment, "FRAGMENT");

        unsigned int geometry;
        if(geometryPath[0] != '\0')
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            GLCall(glShaderSource(geometry, 1, &gShaderCode, nullptr));
            GLCall(glCompileShader(geometry));
            checkCompileErrors(geometry, "GEOMETRY");
        }

        ID = glCreateProgram();
        GLCall(glAttachShader(ID, vertex));
        GLCall(glAttachShader(ID, fragment));
        if (geometryPath[0] != '\0') {
            GLCall(glAttachShader(ID, geometry));
        }
        GLCall(glLinkProgram(ID));
        checkCompileErrors(ID, "PROGRAM");

        GLCall(glDeleteShader(vertex));
        GLCall(glDeleteShader(fragment));
        if(geometryPath[0] != '\0') {
            GLCall(glDeleteShader(geometry));
        }
    }

    void use()
    {
        GLCall(glUseProgram(ID));
    }

    void setBool(const std::string &name, bool value) const
    {
        GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
    }

    void setInt(const std::string &name, int value) const
    {
        GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
    }

    void setFloat(const std::string &name, float value) const
    {
        GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        GLCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
    }
    void setVec2(const std::string &name, float x, float y) const

    {
        GLCall(glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y));
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        GLCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z));
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
    }

    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w));
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }

private:
    void checkCompileErrors(GLuint shader, const std::string &type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
            if(!success)
            {
                GLCall(glGetShaderInfoLog(shader, 1024, nullptr, infoLog));
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
            if(!success)
            {
                GLCall(glGetProgramInfoLog(shader, 1024, nullptr, infoLog));
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string geometryShaderPath;
};

#endif //PROXIMA_SHADER_H
