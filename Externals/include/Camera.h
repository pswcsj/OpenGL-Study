#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shaderClass.h"

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f); //카메라에서 물체를 바라보는 시선
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    bool firstClick = true;
    int width;
    int height;
    
    float speed = 0.1f; //카메라 속도
    float sensitivity = 100.0f; //카메라 민감도
    
    Camera(int width, int height, glm::vec3 position);
    
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    // view, projection Matrix를 shader에 전달하는 함수
    void Inputs(GLFWwindow* window); //모든 입력값들을 다루는 함수
};

#endif
