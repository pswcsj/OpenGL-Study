#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

/*
 각각의 점에 대해 색이 모두 다르면 OpenGL이 자동으로 그래디언트 처리를 해줌(interpolation)
 */

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
    -0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,  5.0f, 0.0f,
     0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
     0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
    0.0f, 0.8f, 0.0f,     0.92f, 0.86f, 0.76f,    2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
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
    
    GLFWwindow *window = glfwCreateWindow(width, height, "Test", NULL, NULL);
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
    glViewport(0,0, width, height); //OpenGL이 render하고 싶은 영역 (0,0)(top right)에서 (800, 800)까지
    
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
    
    Texture TEX1("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    TEX1.texUnit(shaderProgram, "tex0", 0);
    
    float rotation = 0.0f; //회전 각(육십분법)
    double prevTime = glfwGetTime(); //현재 시간
    
    glEnable(GL_DEPTH_TEST); //삼각형의 깊이를 만들어줌
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //바뀔 버퍼(Back Buffer) 설정
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //현재 버퍼(Front Buffer)를 Back Buffer로 초기화(color, depth 둘 다)
    glfwSwapBuffers(window);
    
    while (!glfwWindowShouldClose(window)) //window가 꼭 닫혀야 하는 상황(Close 버튼, 다른 명령으로 종료 등)이 아니면 실행
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();
        
        double crntTime = glfwGetTime();
        if(crntTime - prevTime >= 1 / 60) //1/60초마다 회전
        {
            rotation += 0.5f;
            prevTime = crntTime;
        }
        //4차원을 사용하는 이유는 이동 행렬의 경우 3차원 공간을 이동하려면 4차원 행렬이 필요하기 때문
        glm::mat4 model = glm::mat4(1.0f); // 4차원 정사각 단위행렬 생성(기본값은 0)
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f); // 임의 좌표(-1000~1000) -> NDC(-1, 1)
        
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); //회전각 설정, 회전축 설정(y축 회전시킬 거니까 y쪽에 1 대입)
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f)); // 이동 변환
        proj = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f); // 원근법 (FOV, 가로/세로, 가까운 면까지의 거리, 먼 면까지의 거리)
        
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //(uniform 위치, 수정될 요소의 개수(배열일 경우 1이상 가능), 행렬 저장 방식(열중심, 행중심), 행렬 변수의 포인터값)
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        
        
        glUniform1f(uniID, 0.5f); //shaderProgram을 활성화한 후에 uniform을 사용할 수 있음
        TEX1.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0); //도형 종류, indices 개수, indices 데이터 타입, indices의 시작 index
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

