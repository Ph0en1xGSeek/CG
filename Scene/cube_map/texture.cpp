#include <iostream>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <GL\freeglut.h>
#include "texture.h"
#include <GL/glext.h>

using namespace std;


unsigned int LoadTex(char* Image)
{
    unsigned int Texture;

    FILE* img = NULL;
    img = fopen(Image,"rb");

    unsigned long bWidth = 0;
    unsigned long bHeight = 0;
    DWORD size = 0;

    fseek(img,18,SEEK_SET);
    fread(&bWidth,4,1,img);
    fread(&bHeight,4,1,img);
    fseek(img,0,SEEK_END);
    size = ftell(img) - 54;

    unsigned char *data = (unsigned char*)malloc(size);

    fseek(img,54,SEEK_SET);    // image data
    fread(data,size,1,img);

    fclose(img);

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

    if (data)
        free(data);

    return Texture;
}


//����һ��.bmp��ʽ����ͼ����
int LoadGLTextures(GLuint& unTexture, char* chFileName)
{
    unTexture = LoadTex(chFileName);               // ����һ������unTexture
    glBindTexture(GL_TEXTURE_2D, unTexture);        //������Ȼ��Ը������������������
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //�����˲�Ϊ�����˲�
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    //�����˲�
    return 1;
}
