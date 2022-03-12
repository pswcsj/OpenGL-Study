#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
    Model(const char* file);
    
    void Draw
    (
        Shader& shader, Camera& camera);
private:
    const char* file;
    std::vector<unsigned char> data; //모델의 데이터가 가지는 데이터의 바이트들의 벡터
    json JSON;
    
    std::vector<Mesh> meshes; //로드된 mesh를 저장할 벡터
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes; //quat : quaternion
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;
    
    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;
    
    void loadMesh(unsigned int indMesh);
    
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));
    //node들을 순회하며 rotation, translation, scale 등의 정보를 가져옴
    
    std::vector<unsigned char> getData(); //bin 파일 받아오는 함수
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTextures();
    
    std::vector<Vertex> assembleVertices
    (
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs
    );
    
    //getFloats에서 vector에 대한 정보를 담은 floats들을 벡터로 묶어줌 (2,3 -> vec2(2, 3))
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
    
};
#endif
