#include "EBO.h"

EBO::EBO(std::vector<GLuint>& indices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);//EMO를 바인딩 GL_ELEMENT_ARRAY_BUFFER는  GL_ARRAY_BUFFER의 indices를 나타내는 데이터에 쓰임
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
};

void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);//EMO를 바인딩 GL_ELEMENT_ARRAY_BUFFER는  GL_ARRAY_BUFFER의 indices를 나타내는 데이터에 쓰임
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//EMO를 바인딩 GL_ELEMENT_ARRAY_BUFFER는  GL_ARRAY_BUFFER의 indices를 나타내는 데이터에 쓰임
}

void EBO::Delete()
{
    glDeleteBuffers(1, &ID);
}
