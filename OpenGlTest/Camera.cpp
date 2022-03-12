#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}


void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    view = glm::lookAt(Position, Position+Orientation, Up);
    //(eye, center, up) =(카메라 뷰포인트의 위치, 바라볼 곳의 위치, 그 세계에서 위쪽을 가리키는 단위벡터)
    projection = glm::perspective(glm::radians(FOVdeg), (float)(width/height), nearPlane, farPlane);
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char *uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
    //버튼이 동시에 눌렸을 때 사선으로 가게하기 위해 if문을 사용
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position -= speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        std::cout<<glm::to_string(Position) <<std::endl;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) //만약 마우스 왼쪽 버튼이 눌리면
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // 마우스 커서 사라지게
        
        if(firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY); //마우스 커서 위치를 찾아 mouseX, mouseY에 기록
        
        float rotX = sensitivity * (float)(mouseY - (height / 2))/height; //(마우스가 중간에서 떨어진 길이)/높이(정규화를 위해 사용)
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;
        
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up))); // Orientation을 -rotX만큼 회전, 회전축은 Orientation과 Up의 외적값(X축)
        if(!((glm::angle(newOrientation, Up) <= glm::radians(5.0f) or glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
        { //고개가 너무 들렸거나 너무 내려가지 않았다면
            Orientation = newOrientation;
        }
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
        glfwSetCursorPos(window, (width / 2), (height / 2));
        
        
        
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) //만약 마우스 왼쪽 버튼이 풀리면
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 마우스 커서 나타나게
        firstClick = true;
    }
    
}
