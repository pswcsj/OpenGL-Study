#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &ID); //VBO 객체 생성 1st: 만들 vbo 개수 2nd: 생성된 vbo들의 번호를 저장할 배열 시작 주소
    glBindBuffer(GL_ARRAY_BUFFER, ID); //VBO의 버퍼를 현재 화면에 적용되게하기 위해 바인딩함
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);//바인딩한 버퍼(VBO)에 데이터(vertices) 저장 (버퍼 종류, 버퍼 사이즈, 버퍼 시작 주소, 버퍼 사용 방법)
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID); //VBO의 버퍼를 현재 화면에 적용되게하기 위해 바인딩함
}
void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO의 버퍼를 현재 화면에 적용되게하기 위해 바인딩함
}
void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}
