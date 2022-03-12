#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 crntPos; //오브젝트(피라미드)의 위치
out vec3 Normal; //법선 벡터
out vec3 color; //color 출력
out vec2 texCoord;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
//    crntPos = vec3(model * vec4(aPos, 1.0f)); //월드를 기준으로 한 현재 위치
    crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f)); //왜 rotation에 -취하지?
    Normal = aNormal;
    color = aColor; // Assigns the colors from the Vertex Data to "color"
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex; //오류 해결 위해 x축의 양의 방향으로 90도 만큼 회전
    
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}
