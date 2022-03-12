#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color; //vertexShader로부터 가져옴
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
    vec3 lightVec = lightPos - crntPos; //물체에서 광원까지의 벡터
    float dist = length(lightVec); //물체에서 광원까지의 거리
    float a = 3.0;
    float b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f); //거리에 따른 빛의 세기 변화 구현
    
    float ambient = 0.20f; //빛이 직접적으로 비추지 않는 부분도 빛의 회절성 때문에 보인다는 것을 구현한 값
    vec3 normal = normalize(Normal); //법선 벡터
    vec3 lightDirection = normalize(lightVec); //물체에서 광원까지의 방향벡터
    
    float diffuse = max(dot(normal, lightDirection), 0.0f); //법선 벡터와 빛의 방향벡터가 이루는 각의 크기가 작으면 더 밝다는 것을 구현. 법선벡터, 빛방향 벡터가 이루는 코사인 값과 0 중 큰 걸로 선택
    
    //specular은 광원과 가까이 있는 부분은 특히 더 밝게 빛나는 것을 구현한 것임.
    float specularLight = 0.50f; //specular의 최댓값
    vec3 viewDirection = normalize(camPos - crntPos); //물체에서 카메라로의 방향 벡터
    vec3 reflectionDirection = reflect(-lightDirection, normal); //reflect(물체에 투사된 빛, 법선 벡터)
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    //해당 영역에 비출 빛의 양 더 작은 것은 빠르게 줄어들게 해서 빛이 집중되는 효과를 위해 제곱승을 사용 빛이 반사되는 구간에서 빛이 제일 많고 각이 커질수록 빛이 어두워지는 구조이니 내적값을 사용
    
    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse*inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor; //텍스처 객체, 텍스처 좌표
    //glsl에서 벡터끼리의 *연산은 좌표끼리 곱한 값을을 원소로 하는 새로운 벡터
    //        행렬, 벡터와 스칼라의 사칙연산은 각 원소에 사칙연산을 하는 방식으로 이루어짐
}

vec4 directLight()
{
    float ambient = 0.20f; //빛이 직접적으로 비추지 않는 부분도 빛의 회절성 때문에 보인다는 것을 구현한 값
    vec3 normal = normalize(Normal); //법선 벡터
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f)); // 빛 방향의 반대 방향

    float diffuse = max(dot(normal, lightDirection), 0.0f); //법선 벡터와 빛의 방향벡터가 이루는 각의 크기가 작으면 더 밝다는 것을 구현. 법선벡터, 빛방향 벡터가 이루는 코사인 값과 0 중 큰 걸로 선택

    //specular은 광원과 가까이 있는 부분은 특히 더 밝게 빛나는 것을 구현한 것임.
    float specularLight = 0.50f; //specular의 최댓값
    vec3 viewDirection = normalize(camPos - crntPos); //물체에서 카메라로의 방향 벡터
    vec3 reflectionDirection = reflect(-lightDirection, normal); //reflect(물체에 투사된 빛, 법선 벡터)
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    //해당 영역에 비출 빛의 양 더 작은 것은 빠르게 줄어들게 해서 빛이 집중되는 효과를 위해 제곱승을 사용 빛이 반사되는 구간에서 빛이 제일 많고 각이 커질수록 빛이 어두워지는 구조이니 내적값을 사용

    float specular = specAmount * specularLight;
    return (texture(diffuse0, texCoord) * (diffuse+ambient)+texture(specular0, texCoord).r * specular) * lightColor; //텍스처 객체, 텍스처 좌표
}

vec4 spotLight()
{
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    float ambient = 0.2f; //빛이 직접적으로 비추지 않는 부분도 빛의 회절성 때문에 보인다는 것을 구현한 값
    vec3 normal = normalize(Normal); //법선 벡터
    vec3 lightDirection = normalize(lightPos - crntPos); //물체에서 광원까지의 방향벡터
    
    float diffuse = max(dot(normal, lightDirection), 0.0f); //법선 벡터와 빛의 방향벡터가 이루는 각의 크기가 작으면 더 밝다는 것을 구현. 법선벡터, 빛방향 벡터가 이루는 코사인 값과 0 중 큰 걸로 선택
    
    //specular은 광원과 가까이 있는 부분은 특히 더 밝게 빛나는 것을 구현한 것임.
    float specularLight = 0.5f; //specular의 최댓값
    vec3 viewDirection = normalize(camPos - crntPos); //물체에서 카메라로의 방향 벡터
    vec3 reflectionDirection = reflect(-lightDirection, normal); //reflect(물체에 투사된 빛, 법선 벡터)
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    //해당 영역에 비출 빛의 양 더 작은 것은 빠르게 줄어들게 해서 빛이 집중되는 효과를 위해 제곱승을 사용 빛이 반사되는 구간에서 빛이 제일 많고 각이 커질수록 빛이 어두워지는 구조이니 내적값을 사용
    float specular = specAmount * specularLight;
    
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection); // 밑으로 가는 벡터?와 빛의 벡터가 이루는 각의 코사인
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor; //텍스처 객체, 텍스처 좌표
}
void main()
{
    FragColor = directLight();
}
