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
#include "Camera.h"

/*
 각각의 점에 대해 색이 모두 다르면 OpenGL이 자동으로 그래디언트 처리를 해줌(interpolation)
 */

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,      0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,      0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,      0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
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
    
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag"); //vertical, fragment 관련된 파일 불러옴
    
    VAO VAO1;
    VAO1.Bind();
    
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));
    
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11*sizeof(float), (void*)(0));
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11*sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11*sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11*sizeof(float), (void*)(8 * sizeof(float)));
    
    VAO1.Unbind();
    VBO1.Unbind(); //사실 LinkAttrib에서 이미 Unbind돼서 없어도 됨
    EBO1.Unbind();
    
    
    Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

    VAO lightVAO;
    lightVAO.Bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)(0));

    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();
    
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //
    
    //light, pyramid 각각 위에서 설정한 vertices 값에서 평행이동 시키기위해 model 행렬 설정
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos); //x, y, z로 0.5만큼 평행이동하는 변환행렬
    
    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos); // 평행이동하지 않음
    
    lightShader.Activate(); //lightShader Uniform에 접근하기 위해 활성화
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel)); //model 행렬을 lightModel로 설정
    shaderProgram.Activate(); //shaderProgram Uniform에 접근하기 위해 활성화
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel)); //model 행렬을 pyramidModel로 설정
    
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Texture TEX1("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE); //텍스처 생성
    TEX1.texUnit(shaderProgram, "tex0", 0); //tex0(텍스처 레퍼런스)값을 fragment Shader에 넘겨줘서 텍스처 정보를 넘겨줌
    
    glEnable(GL_DEPTH_TEST); //삼각형의 깊이를 만들어줌
    
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    while (!glfwWindowShouldClose(window)) //window가 꼭 닫혀야 하는 상황(Close 버튼, 다른 명령으로 종료 등)이 아니면 실행
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//바뀔 버퍼(Back Buffer)(배경색) 설정
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//현재 버퍼(Front Buffer)를 Back Buffer로 초기화(color, depth 둘 다)
        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Inputs(window); //입력을 받음
        camera.updateMatrix(45.0f, 0.1f, 100.0f); //Inputs 함수를 통해 변경된 위치를 updateMatrix와 Matrix를 통해 적용
        camera.Matrix(shaderProgram, "camMatrix");
        
        TEX1.Bind(); //직접적으로 texture가 vao나 vbo와 연결되는 부분이 없어서 연결이 끊어져도 사용 가능하지 않은거 같음. bind시켜야 사용 가능한 거 같음
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0); //도형 종류, indices 개수, indices 데이터 타입, indices의 시작 index
        
        lightShader.Activate(); //lightShader을 Activate 시킴으로써 shaderProgram과는 연결이 끊어짐.
        camera.Matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(int), GL_UNSIGNED_INT, 0); //도형 종류, indices 개수, indices 데이터 타입, indices의 시작 index
        
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

