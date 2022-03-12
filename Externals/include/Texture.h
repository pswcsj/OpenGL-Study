#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"

class Texture
{
public:
    GLuint ID;
    const char* type;
    GLuint unit;
    
    Texture(const char* image, const char* texType, GLuint slot);
    //Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);
    //image : 파일명, texType : 텍스쳐 타입(diffuse, specular), slot : 활성화할 텍스쳐 유닛, format : 이미지 포맷(rgb, rgba), pixelType : 픽셀 데이터 타입(unsigned byte)
    void texUnit(Shader &shader, const char* uniform, GLuint unit); //텍스처 유닛 활성화
    void Bind();
    void Unbind();
    void Delete();
};

#endif
