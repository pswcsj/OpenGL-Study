#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    
    VAO VAO;
    
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
    
    void Draw
    (
        Shader& shader,
        Camera& camera,
        glm::mat4 matrix = glm::mat4(1.0f),
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), //이유는 모르겠는데 quaternion 에서 저거일 때 영향이 없나봄
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
    ); // matrix, translation, rotation 모두 적용해도 영향을 주지 않는 값이 기본값
};
#endif
