#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID); //1개의 VAO객체 생성
}

void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset); //https://heinleinsgame.tistory.com/8
    glEnableVertexAttribArray(layout); //vertexAttrib에서 0을 활성화
    VBO.Unbind();
}

void VAO::Bind()
{
    glBindVertexArray(ID);
}
void VAO::Unbind()
{
    glBindVertexArray(0);
}
void VAO::Delete()
{
    glDeleteVertexArrays(1, &ID);
}
