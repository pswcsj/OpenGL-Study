#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //vertex-shader 객체의 주소값? 생성 https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glCreateShader.xml
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //vertexShader 객체의 소스코드를 설정해줌. 두번째의 파라미터는 아마 문자열 배열이 몇개인지?를 가리키는 듯 https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
    glCompileShader(vertexShader); //위에서 설정한 소스코드를 컴파일
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    GLuint shaderProgram = glCreateProgram(); //shaderProgram은 vertexShader, fragmentShader를 묶어줌
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader); //shaderProgram에 각각을 붙임
    glLinkProgram(shaderProgram); //shader들을 링크한다는데 아마 실행하는 듯?
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); //Attach된 shader들을 떼어냄
    
    // Vertices coordinates
        GLfloat vertices[] =
        {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
            -0.5f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
        };
    
    
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); //1개의 VAO객체 생성
    glGenBuffers(1, &VBO); //VBO 객체 생성 1st: 만들 vbo 개수 2nd: 생성된 vbo들의 번호를 저장할 배열 시작 주소
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //VBO의 버퍼를 현재 화면에 적용되게하기 위해 바인딩함
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//버퍼(데이터)들을 VBO에 저장 (버퍼 종류, 버퍼 사이즈, 버퍼 시작 주소, 버퍼 사용 방법)
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //https://heinleinsgame.tistory.com/8
    glEnableVertexAttribArray(0); //vertexAttrib에서 0을 활성화
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0); //실수로라도 VAO, VBO를 바꾸지 않기 위해 0에 바인드시킴
    
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //바뀔 버퍼(Back Buffer) 설정
    glClear(GL_COLOR_BUFFER_BIT); //현재 버퍼(Front Buffer)를 Back Buffer로 초기화
    glfwSwapBuffers(window);
    
    while (!glfwWindowShouldClose(window)) //window가 꼭 닫혀야 하는 상황(Close 버튼, 다른 명령으로 종료 등)이 아니면 실행
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram); //shader들을 적용
        glBindVertexArray(VAO); //아까 VertexArray 해제시켰으니 다시 바인드
        glDrawArrays(GL_TRIANGLES, 0, 3); //점들의 시작 위치, 값 개수
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}
