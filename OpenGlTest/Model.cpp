#include "Model.h"

Model::Model(const char* file)
{
    std::string text = get_file_contents(file);
    JSON = json::parse(text); //JSON 파일을 Dictionary 구조로 변환시킴
    
    Model::file = file; //파일 이름 저장
    data = getData();
    
    traverseNode(0); //0부터 시작해서 끝까지 순회
};

void Model::Draw(Shader& shader, Camera& camera)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
    }
}
void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
    json node = JSON["nodes"][nextNode];
    
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end()) //만약 translation이 존재하면
    {
        float transValues[3];
        //translation 좌표들을 받아옴
        for (unsigned int i = 0; i < node["translation"].size(); i++)
            transValues[i] = node["translation"][i];
        translation = glm::make_vec3(transValues); //3개짜리 float 배열을 vec3 형식으로 바꿔줌
        
    }
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end())
    {
        float rotValues[4] =
        {
            node["rotation"][3],
            node["rotation"][0],
            node["rotation"][1],
            node["rotation"][2]
        }; //gltf는 quaternions를 x, y, z, w 순으로 하지만, glm은 w, x, y, z순으로 사용하기 때문에 이렇게 함
        rotation = glm::make_quat(rotValues);
    }
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end())
    {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++)
            scaleValues[i] = node["scale"][i];
        scale = glm::make_vec3(scaleValues);
    }
    glm::mat4 matNode = glm::mat4(1.0f); //이건 왜 만들거지?
    if (node.find("matrix") != node.end())
    {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++)
        {
            matValues[i] = node["matrix"][i];
        }
        matNode = glm::make_mat4(matValues);
    }
    
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);
    
    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);
    
    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca; //아마 scale>rot>trans>matrix 순으로 적용 순서인가봄
    
    if (node.find("mesh") != node.end()) //만약 mesh 키를 가진 게 존재하면
    {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);
        
        loadMesh(node["mesh"]);
    }
    
    if (node.find("children") != node.end())
    {
        for (unsigned int i = 0; i < node["children"].size(); i++)
        {
            traverseNode(node["children"][i], matNextNode);
        }
    }
    
}
std::vector<unsigned char> Model::getData()
{
    std::string bytesText; //bytetext를 저장함
    std::string uri = JSON["buffers"][0]["uri"]; //vertex, indices 등의 기하학적 정보를 담은 .bin 파일의 주소
    
    std::string fileStr = std::string(file); //파일명
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/')+1); //파일명을 제외한 파일 경로 저장(파일 바로 위 경로 디렉토리 이름)
    bytesText = get_file_contents((fileDirectory + uri).c_str());
    //예를 들어 file이 abc/abc.gltf이고 uri가 abc.bin이면 fileDirectory = abc/이니 fileDirectory+uri=abc/abc.bin 따라서 abc.bin파일에 접근하여 이를 바이트 파일로 받아옴
    
    std::vector<unsigned char> data(bytesText.begin(), bytesText.end()); //begin()부터 end() 원소까지 순회하며 vector에 값을 할당
    return data;
}

void Model::loadMesh(unsigned int indMesh)
{
    unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"]; //accessors의 몇번 인덱스가 position을 가리키는지
    unsigned int norAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"]; //법선 벡터
    unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"]; //텍스처 좌표
    unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"]; //indices
    
    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<float> normalVec = getFloats(JSON["accessors"][norAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);
    
    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();
    
    meshes.push_back(Mesh(vertices, indices, textures));
}

std::vector<float> Model::getFloats(json accessor) //accessors에 저장되어 있는 accessor 원소 중 하나
{
    std::vector<float> floatVec;
    
    unsigned int buffViewInd = accessor.value("bufferView", 1); //우리가 사용할 bufferView index를 알려줌
    //bufferView가 존재하면 찾아서 주고 존재X면 1 반환(보편적으로 indices와 다른 것 하나만 있으면 그 다른 것은 1인 거 같음)
    unsigned int count = accessor["count"]; //얻어야하는 요소의 개수
    //무조건 존재하기 때문에 중괄호 사용
    unsigned int accByteOffset = accessor.value("byteOffset", 0); //bufferView에서 현재 사용할 데이터까지 벌어진 차이
    std::string type = accessor["type"]; //float, vec2 등의 사용할 데이터 타입(어떻게 float들을 그루핑할지 알려줘야하기 때문에)
    
    json bufferView = JSON["bufferViews"][buffViewInd]; //얻고싶은 bufferView에 관한 json을 받아옴
    unsigned int byteOffset = bufferView["byteOffset"]; //bufferView가 buffer(bin파일)의 어디부터 시작하는지
    
    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, VEC4)");
    
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert; //요소 개수 * float 사이즈 * 요소에 사용된 float의 개수
    
    for(unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
    {
        unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]}; //float에 저장할거기 때문에 4바이트로 맞춘 것
        float value;
        std::memcpy(&value, bytes, sizeof(float)); //value에 bytes에 있는 메모리를 복사
        floatVec.push_back(value); //value를 floatVec에 추가
    }
    
    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
    std::vector<GLuint> indices;
    
    unsigned int buffViewInd = accessor.value("bufferView", 0); //보편적으로 indices는 0번 bufferView를 쓰는 거 같음
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];
    
    json bufferView = JSON["bufferViews"][buffViewInd]; //얻고싶은 bufferView에 관한 json을 받아옴
    unsigned int byteOffset = bufferView["byteOffset"]; //데이터에서 bufferview가 어디서부터 시작하는지
    
    unsigned int beginningOfData = byteOffset + accByteOffset;
    
    if(componentType == 5125) //unsigned int
    {
        // unsigned int는 4바이트이니 
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i)
        {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]}; //float에 저장할거기 때문에 4바이트로 맞춘 것
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int)); //value가 가리키는 주소값을 bytes로?
            indices.push_back((GLuint)value); //value를 floatVec에 추가
        }
    }
    
    else if(componentType == 5123) //unsigned short
    {
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i)
        {
            unsigned char bytes[] = {data[i++], data[i++]};
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
            
        }
    }
    
    else if(componentType == 5122) //short
    {
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i)
        {
            unsigned char bytes[] = {data[i++], data[i++]};
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
            
        }
    }
    return indices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
    std::vector<glm::vec2> vectors;
    
    for(unsigned int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
    std::vector<glm::vec3> vectors;
    
    for(unsigned int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
    std::vector<glm::vec4> vectors;
    
    for(unsigned int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<Vertex> Model::assembleVertices
(
    std::vector<glm::vec3> positions,
    std::vector<glm::vec3> normals,
    std::vector<glm::vec2> texUVs
)
{
    std::vector<Vertex> vertices;
    for(int i = 0; i < positions.size(); i++)
    {
        vertices.push_back
        (
            Vertex
            {
                positions[i],
                normals[i],
                glm::vec3(1.0f, 1.0f, 1.0f),
                texUVs[i]
            }
         );
    }
    return vertices;
}

std::vector<Texture> Model::getTextures()
{
    std::vector<Texture> textures; //Texture 벡터 정의
    
    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/')+1); //파일 경로 전체에서 가장 마지막 '/'까지 잘라낸 것

    bool skip = false;
    
    for(int i = 0; i < JSON["images"].size(); i++)
    {
        std::string texPath = JSON["images"][i]["uri"];
        
        for(int j = 0; j < loadedTexName.size(); j++)
        {
            if(loadedTexName[j].find(texPath) == std::string::npos) //이미 저장된 텍스처를 또 저장하는 것을 방지하기 위해 사용
            {
                textures.push_back(loadedTex[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            if(texPath.find("baseColor") != std::string::npos) //npos는 texPath에서 "baseColor"라는 문자열을 찾지 못했을 경우의 반환값 npos=-1 //baseColor은 diffuse를 의미
            {
                Texture diffuse = Texture((fileDirectory+texPath).c_str(), "diffuse", loadedTex.size());
                textures.push_back(diffuse);
                loadedTex.push_back(diffuse);
                loadedTexName.push_back(texPath);
            }
            else if(texPath.find("metallicRoughness") != std::string::npos) //metallicRoughness는 specular을 의미
            {
                Texture specular = Texture((fileDirectory+texPath).c_str(), "specular", loadedTex.size());
                textures.push_back(specular);
                loadedTex.push_back(specular);
                loadedTexName.push_back(texPath);
            }
        }
    }
    return textures;
}

