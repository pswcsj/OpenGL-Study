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
#include "Mesh.h"
#include "Model.h"
/*
 각각의 점에 대해 색이 모두 다르면 OpenGL이 자동으로 그래디언트 처리를 해줌(interpolation)
 */

const unsigned int width = 800;
const unsigned int height = 800;

//// Vertices coordinates
//Vertex vertices[] =
//{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
//    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
//    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
//    Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
//    Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
//};
//
//
//// Indices for vertices order
//GLuint indices[] =
//{
//    0, 1, 2,
//    0, 2, 3
//};
//
//Vertex lightVertices[] =
//{ //     COORDINATES     //
//    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
//    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
//    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
//    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
//    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
//    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
//    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
//    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
//};
//
//GLuint lightIndices[] =
//{
//    0, 1, 2,
//    0, 2, 3,
//    0, 4, 7,
//    0, 7, 3,
//    3, 7, 6,
//    3, 6, 2,
//    2, 6, 5,
//    2, 5, 1,
//    1, 5, 4,
//    1, 4, 0,
//    4, 5, 6,
//    4, 6, 7
//};

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
    
//    Texture textures[] =
//    {
//        Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
//        Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
//    };
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag"); //vertical, fragment 관련된 파일 불러옴
//    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex)); //vertices 시작주소부터 끝주소까지
//    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
//    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    
//    Mesh floor(verts, ind, tex);
//
//    Shader lightShader("Shaders/light.vert", "Shaders/light.frag");
////
//    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
//    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
//
//    Mesh light(lightVerts, lightInd, tex); //tex는 안쓰지만 주는거
    
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //
//
//    //light, pyramid 각각 위에서 설정한 vertices 값에서 평행이동 시키기위해 model 행렬 설정
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos); //x, y, z로 0.5만큼 평행이동하는 변환행렬
//
//    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::mat4 pyramidModel = glm::mat4(1.0f);
//    pyramidModel = glm::translate(pyramidModel, pyramidPos); // 평행이동하지 않음
    
//    lightShader.Activate(); //lightShader Uniform에 접근하기 위해 활성화
//    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel)); //model 행렬을 lightModel로 설정
//    shaderProgram.Activate(); //shaderProgram Uniform에 접근하기 위해 활성화
//    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel)); //model 행렬을 pyramidModel로 설정
//
    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    glEnable(GL_DEPTH_TEST); //삼각형의 깊이를 만들어줌
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    
    Model model1("bunny/scene.gltf");
    while (!glfwWindowShouldClose(window)) //window가 꼭 닫혀야 하는 상황(Close 버튼, 다른 명령으로 종료 등)이 아니면 실행
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//바뀔 버퍼(Back Buffer)(배경색) 설정
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//현재 버퍼(Front Buffer)를 Back Buffer로 초기화(color, depth 둘 다)
        camera.Inputs(window); //입력을 받음
        camera.updateMatrix(45.0f, 0.1f, 100.0f); //Inputs 함수를 통해 변경된 위치를 updateMatrix와 Matrix를 통해 적용
        
//        floor.Draw(shaderProgram, camera);
//        light.Draw(lightShader, camera);
        model1.Draw(shaderProgram, camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shaderProgram.Delete();
//    lightShader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


