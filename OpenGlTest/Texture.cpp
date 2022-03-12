#include <Texture.h>

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
    //Texture
    int widthImg, heightImg, numColCh; //가로, 높이, color channels(rgb, rgba) 개수?
    type = texType;
    stbi_set_flip_vertically_on_load(true); //stb, OpenGL의 이미지 읽는 방식의 차이때문에 이미지가 뒤집히는 것을 방지하고 위해 사용
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0); //이미지를 바이트 형식으로 저장 // 이미지를 읽어와서 너비, 높이, 컬러 채널 개수를 받아와 변수에 매핑하는 거 같음
    
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0+slot); //활성화할 Texture 유닛
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID); //Texture를 바인드
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //텍스처 설정 조정하는 함수 (텍스처 타입, 수정하고 싶은 세팅, 수정될 세팅값) min_filter값을 gl_nearest로 설정 gl_texture_min_filter : 이미지가 축소됐을 때 어떤 필터를 적용할지
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //확대됐을 때
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //gl_texture_wrap : 이미지에서 남은 부분을 어떻게 채울지
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // gl_texture_border_color을 설정하려면 이렇게 해야함
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor)
    
    if(numColCh == 4) //컬러 채널 개수가 4이면(RGBA)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if(numColCh == 3)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else if(numColCh == 1)
    {
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }
    else
        throw std::invalid_argument("Automatic Texture type recognition failed");
    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    //데이터를 현재 바인드된 텍스처로 업로드
    //텍스처 타입, 생성하는 텍스처의 mipmap 레벨(0이 베이스 레벨), 텍스처가 갖길 바라는 이미지 채널, 가로, 세로, 0(관례), 우리 이미지가 갖고 있는 이미지 채널(jpeg:rgb, png:rgba), 픽셀 데이터 타입, 이미지 데이터
    
    glGenerateMipmap(GL_TEXTURE_2D); //여러 해상도의 텍스처를 만듦(픽셀 뭉침 등의 현상 완화 위해)
    
    stbi_image_free(bytes); //이미지 데이터 삭제
    glBindTexture(GL_TEXTURE_2D, 0); //텍스처 이미지 언바인드
}

void Texture::texUnit(Shader &shader, const char* uniform, GLuint unit) 
{
    GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(tex0Uni, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

