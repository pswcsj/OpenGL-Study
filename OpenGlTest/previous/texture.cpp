#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <iostream>
#include <cmath>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

/*
 각각의 점에 대해 색이 모두 다르면 OpenGL이 자동으로 그래디언트 처리를 해줌(interpolation)
 */


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,// Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  0.0f, 1.0f,// Upper left corner
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,// Upper right corner
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    1.0f, 0.0f// Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
    0, 2, 1, // Upper triangle
    0, 3, 2 // Lower triangle
};

int main()
{
    glfwInit(); //glfw 초기화
    //glfw의 버전을 OpenGL에게 알려주는 코드
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //OpenGL 패키지를 임포트 한다는 느낌?
    
    //만약 __APPLE__이 정의되어 있다면 즉, 맥 OS에서 동작한다면 밑의 코드를 소스코드에 넣는다
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "Test", NULL, NULL);
    //윈도우 객체 생성 glfwCreateWindow(width, height, name, Full Screen?, ?);
    
    // 윈도우 객체 생성에 실패했을 때
    if (!window)
    {
        std::cout << "Failed to create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //glfw에게 window 객체를 사용한다고 말하는 것
    gladLoadGL(); //필요한 configurations들을 load하라 명령
    glViewport(0,0, 800, 800); //OpenGL이 render하고 싶은 영역 (0,0)(top right)에서 (800, 800)까지
    
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    
    VAO VAO1;
    VAO1.Bind();
    
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));
    
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)(0));
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6 * sizeof(float)));
    
    VBO1.Unbind();
    VAO1.Unbind();
    EBO1.Unbind();
    
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); //scale uniform의 참조값을 uniID에 받아옴
    
    Texture TEX1("image.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    TEX1.texUnit(shaderProgram, "tex0", 0);
    
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //바뀔 버퍼(Back Buffer) 설정
    glClear(GL_COLOR_BUFFER_BIT); //현재 버퍼(Front Buffer)를 Back Buffer로 초기화
    glfwSwapBuffers(window);
    
    while (!glfwWindowShouldClose(window)) //window가 꼭 닫혀야 하는 상황(Close 버튼, 다른 명령으로 종료 등)이 아니면 실행
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f); //shaderProgram을 활성화한 후에 uniform을 사용할 수 있음
        TEX1.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //도형 종류, indices 개수, indices 데이터 타입, indices의 시작 index
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    TEX1.Delete();
    glfwTerminate();
    return 0;
}

