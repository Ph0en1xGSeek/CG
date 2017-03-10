#include <iostream>
#include <windows.h>
#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <string>
#include <GL\glext.h>
#include "robot.h"
#include "texture.h"
#include <time.h>
#include "tga.h"		/* simple TGA image file loader */
#include "trackball.h"	/* trackball quaternion routines */
#include <assert.h>
#include <conio.h>
#include <time.h>

using namespace std;

GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zRot = 0;
GLfloat xOrigin = -1, yOrigin = -1, xrOrigin = -1, yrOrigin = -1;
GLfloat deltaAngley = 0, deltaAnglex = 0;
GLint fogMode;
bool mipmaps = true;
int mode = GL_REFLECTION_MAP;
int wrap = GL_CLAMP_TO_EDGE;

//随机数
int ran = 1;
bool ran_flag = true;

bool active = true;
bool over = false;

bool aactive[6] = { false };

float x0 = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0,y11 = 0, z1 = 0;
//跳和蹲
bool jump = false;
bool down = false;
int down_num = 0;
//加速度
GLfloat high = 0.3;
//得分
int score = 0;


//速度
float v = 0.45;
bool s1 = true;
bool s2 = true;
bool s3 = true;

bool ss1 = true;
bool ss2 = false;
bool ss3 = false;
bool ss4 = false;

//材质
GLuint bas = 0;
GLuint mc = 0;
GLuint road = 0, back, up, lf, front, rt, dn;
GLuint road1 = 0;
GLuint road22 = 0;
GLuint road33 = 0;

//视角
bool s_w = false, s_a = false, s_d = true;

//直升飞机大螺旋桨旋转
GLfloat Hzot = 0;
//直升飞机小螺旋桨旋转
GLfloat hyot = 0;
//沙漏旋转
GLfloat Xzot = 0;
//正方形旋转
GLfloat Cxot = 0;
GLfloat Czot = 0;
//公路
float rx = 0, ry = 0, rz = 0;
float rx1 = 0;
float rx2 = 0;
float rx3 = 0;
float rx4 = 0;
bool f_r = true, f_r1 = true, f_r2 = false, f_r3 = false;

//雪人
GLuint snowman;

//材质
float no_mat[] = { 0.0,0.0,0.0,1.0 };
float mat_red[] = { 0.8,0.1,0.0,1.0 };
float mat_ambient[] = { 0.3,0.3,0.3,1.0 };
float mat_orange[] = { 1.0,0.2,0.0,1.0 };
float mat_diffuse[] = { 0.5,0.5,0.5,1.0 };
float mat_specular[] = { 1.0,1.0,1.0,1.0 };
float no_shininess[] = { 0.0 };
float low_shininess[] = { 5.0 };
float high_shininess[] = { 100.0 };
float mat_emmision[] = { 0.3,0.2,0.3,0.0 };

//============================================Cube Mapping======================
GLenum faceTarget[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};


/* Pre-generated cube map images. */
char *faceFile[6] = {
	"sincity_lf.tga",
	"sincity_rt.tga",
	"sincity_up.tga",
	"sincity_dn.tga",
	"sincity_bk.tga",
	"sincity_ft.tga",
};
void updateTexgen(void)
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, mode);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, mode);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, mode);
}

void updateWrap(void)
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);
}

void loadFace(GLenum target, char *filename)
{
	FILE *file;
	gliGenericImage *image;

	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("could not open \"%s\"\n", filename);
		exit(1);
	}
	image = gliReadTGA(file, filename);
	fclose(file);

	if (mipmaps) {
		gluBuild2DMipmaps(target, image->components,
			image->width, image->height,
			image->format, GL_UNSIGNED_BYTE, image->pixels);
	}
	else {
		glTexImage2D(target, 0, image->components,
			image->width, image->height, 0,
			image->format, GL_UNSIGNED_BYTE, image->pixels);
	}
}


void makeCubeMap(void)
{
	int i;

	for (i = 0; i<6; i++) {
		loadFace(faceTarget[i], faceFile[i]);
	}
	if (mipmaps) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	updateTexgen();
	updateWrap();
}

//=======================================================

void yuanzhucm(GLfloat r, GLfloat h)//画圆柱侧面
{
	for (GLfloat j = 0; j < 2 * 3.1416; j += 0.01f)
	{
		glBegin(GL_QUADS);
		glVertex3f(r*cos(j), r*sin(j), 0);
		glVertex3f(r*cos(j + 0.01), r*sin(j + 0.01), 0);
		glVertex3f(r*cos(j + 0.01), r*sin(j + 0.01), h);
		glVertex3f(r*cos(j), r*sin(j), h);
		glEnd();
	}
}

void yuanmian(GLfloat r, GLfloat h)//画圆面
{
	for (GLfloat k = 0; k < 2 * 3.1416; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0, 0.0, h);
		glVertex3f(r*cos(k), r*sin(k), h);
		glVertex3f(r*cos(k + 0.05), r*sin(k + 0.05), h);
		glEnd();
	}
}

void box0()
{
	glScalef(1.0, 1.0, 1.5);
	glTranslatef(x0, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(-520.0, 0.0, 9);

	glRotatef(Xzot, 0.0f, 0.0f, 1.0f);
	//两个圆锥
	glPushMatrix();
	glTranslated(0, 0, -8.0);
	glColor3f(0.0f, 0.2f, 0.9f);
	glutSolidCone(1.5, 1.7, 20.0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -4.8);
	glRotated(180, 1.0, 0.0, 0.0);
	glColor3f(0.0f, 0.2f, 0.9f);
	glutSolidCone(1.5, 1.7, 20.0, 1);
	glPopMatrix();


	//上下两个圆盘
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -4.8f);

	glColor3f(0.5, 0.0, 0.0);
	yuanzhucm(2.5, 0.5);
	glColor3f(0.5, 0.0, 0.0);
	yuanmian(2.5, 0.5);
	glColor3f(0.5, 0.0, 0.0);
	yuanmian(2.5, 0.0);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -8.5f);

	glColor3f(0.5, 0.0, 0.0);
	yuanzhucm(2.5, 0.5);
	glColor3f(0.5, 0.0, 0.0);
	yuanmian(2.5, 0.5);

	glPopMatrix();


	//三根柱子
	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, -8.35f);
	glColor3f(0.54, 0.28, 0.15);
	yuanzhucm(0.2, 4);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.0f, 1.732f, -8.35f);
	glColor3f(0.54, 0.28, 0.15);
	yuanzhucm(0.2, 4);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.0f, -1.732f, -8.35f);
	glColor3f(0.54, 0.28, 0.15);
	yuanzhucm(0.2, 4);
	glPopMatrix();

	glPopMatrix();

	Xzot += 1.5f;
	if (Xzot >= 360.0f)
		Xzot = 0.0f;
	glDisable(GL_COLOR_MATERIAL);
}
void box1()
{
	glTranslatef(x1, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(-520.0, 0.0, -5.0);
	glRotated(-90, 0.0, 0.0, 1.0);
	//头
	glPushMatrix();
	glTranslated(0.0, 0.0, 11);
	glColor3f(0.0, 0.7, 0.0);
	glutSolidSphere(3.0, 12, 8);
	glPopMatrix();
	//身子
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 7.5f);
	glColor3f(0.0, 0.4, 0.0);
	yuanzhucm(3.0, 4);
	glColor3f(0.0, 0.4, 0.0);
	yuanmian(3.0, 0);
	glPopMatrix();
	//眼睛
	glPushMatrix();
	glTranslated(1.0, 2.5, 12.5);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(0.2, 12, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.0, 2.5, 12.5);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(0.2, 12, 8);
	glPopMatrix();

	//胳膊
	glPushMatrix();
	glTranslatef(3.4f, 0.0f, 11.0f);
	glRotated(-100, 1.0, 0.0, 0.0);
	glColor3f(0.0, 0.6, 0.0);
	yuanzhucm(0.4, 2.2);
	glColor3f(0.0, 0.6, 0.0);
	yuanmian(0.4, 0);
	glColor3f(0.0, 0.6, 0.0);
	yuanmian(0.4, 2.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.4f, 0.0f, 11.0f);
	glRotated(-140, 1.0, 0.0, 0.0);
	glColor3f(0.0, 0.6, 0.0);
	yuanzhucm(0.4, 2.2);
	glColor3f(0.0, 0.6, 0.0);
	yuanmian(0.4, 0);
	glColor3f(0.0, 0.6, 0.0);
	yuanmian(0.4, 2.2);
	glPopMatrix();

	//天线
	glPushMatrix();
	glTranslatef(1.3f, 0.0f, 13.5f);
	glRotated(40, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.7, 0.0);
	yuanzhucm(0.07, 1.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.3f, 0.0f, 13.5f);
	glRotated(-40, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.7, 0.0);
	yuanzhucm(0.07, 1.1);
	glPopMatrix();

	//腿
	glPushMatrix();
	glTranslatef(1.3f, 0.0f, 5.5f);
	glColor3f(0.0, 0.6, 0.0);
	yuanzhucm(0.4, 2.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.3f, 0.0f, 5.5f);
	glColor3f(0.0, 0.6, 0.0);
	yuanzhucm(0.4, 2.1);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}
void box2()
{
	glTranslatef(x2, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glTranslated(-520.0, 0.0, 3.5 + 13);
	glRotatef(Czot, 0.0f, 0.0f, 1.0f);
	glRotatef(Cxot, 1.0f, 0.0f, 0.0f);
	glColor3f(1, 1, 1);
	glScalef(5, 5, 5);

	glutSolidTeapot(1);
	glDisable(GL_COLOR_MATERIAL);
	Cxot += 1.5f;
	if (Cxot >= 360.0f)
		Cxot = 0.0f;
	Czot += 1.5f;
	if (Czot >= 360.0f)
		Czot = 0.0f;
}
void box3()
{
	glScalef(1.0, 0.7, 0.7);
	glTranslatef(x3, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(-520.0, 0.0, -9.0);
	glRotated(-80, 0.0, 0.0, 1.0);
	//头
	glPushMatrix();
	glTranslated(0.0, 0.0, 20);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(6, 3, 4);
	glutSolidCube(1);
	glPopMatrix();
	//眼睛
	glPushMatrix();
	glTranslated(1.4, 1.6, 20.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.2, 12, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.4, 1.6, 20.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.2, 12, 8);
	glPopMatrix();
	//嘴
	glPushMatrix();
	glTranslated(0.0, 1.6, 18.8);
	glRotated(-90, 1.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidTorus(0.04, 0.2, 20, 20);
	glPopMatrix();
	//身子
	glPushMatrix();
	glTranslated(0.0, 0.0, 16);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(4, 1.5, 7);
	glutSolidCube(1);
	glPopMatrix();
	//胳膊
	glPushMatrix();
	glTranslated(3.6, 0.0, 15.6);
	glRotated(-35, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(1.1, 1.1, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3.6, 0.0, 15.6);
	glRotated(35, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(1.1, 1.1, 4);
	glutSolidCube(1);
	glPopMatrix();
	//裙子
	glPushMatrix();
	glTranslated(0.0, 1.2, 13.0);
	glRotated(30, 1.0, 0.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(4, 0.2, 1.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -1.2, 13.0);
	glRotated(-30, 1.0, 0.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(4, 0.2, 1.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.4, 0.0, 13.0);
	glRotated(-30, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(0.2, 1.5, 1.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.4, 0.0, 13.0);
	glRotated(30, 0.0, 1.0, 0.0);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(0.2, 1.5, 1.3);
	glutSolidCube(1);
	glPopMatrix();
	//腿
	glPushMatrix();
	glTranslated(1.0, 0.0, 11.4);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(1.0, 1.0, 2.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.0, 0.0, 11.4);
	glColor3f(1.0f, 0.63f, 0.26f);
	glScalef(1.0, 1.0, 2.3);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}
void box4()
{
	glTranslatef(x4, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(-520.0, 0.0, 17);

	//身子
	glPushMatrix();
	glColor3f(0.17f, 0.17f, 0.17f);
	glScalef(10, 6, 5);
	glutSolidCube(1);
	glPopMatrix();

	//头
	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mc);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 3.0f, -2.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 3.0f, 2.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 3.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 3.0f, -2.5f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 3.0f, 2.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, -3.0f, 2.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, -3.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 3.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, -3.0f, -2.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, -3.0f, 2.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, -3.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, -3.0f, -2.5f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 3.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, -3.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, -3.0f, -2.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 3.0f, -2.5f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 3.0f, -2.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 3.0f, -2.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, -3.0f, -2.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, -3.0f, -2.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//支架
	glPushMatrix();
	glTranslated(4, 3, -1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-4, 3, -1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, -3, -1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-4, -3, -1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, 4.2, -3);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-4, 4.2, -3);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4, -4.2, -3);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-4, -4.2, -3);
	glColor3f(0.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 3);
	glutSolidCube(1);
	glPopMatrix();


	//两个圆柱
	glPushMatrix();
	glTranslated(-10, 4.2, -4.5);
	glRotated(90, 0.0, 1.0, 0.0);
	yuanzhucm(0.3, 18);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-10, -4.2, -4.5);
	glRotated(90, 0.0, 1.0, 0.0);
	yuanzhucm(0.3, 18);
	glPopMatrix();


	//尾巴
	glPushMatrix();
	glTranslated(-10, 0, 1);
	glColor3f(0.17f, 0.17f, 0.17f);
	glScalef(15, 1.5, 1.5);
	glutSolidCube(1);
	glPopMatrix();


	//大螺旋桨
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslated(0, 0, 1.7);
	yuanzhucm(0.5, 2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(Hzot, 0.0f, 0.0f, 1.0f);
	glTranslated(0, 0, 3.7);
	glScalef(1, 20, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(Hzot, 0.0f, 0.0f, 1.0f);
	glTranslated(0, 0, 3.7);
	glScalef(20, 1, 0.1);
	glutSolidCube(1);
	glPopMatrix();


	//小螺旋桨
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslated(-17, 1.1, 1);
	glRotated(90, 1.0, 0.0, 0.0);
	yuanzhucm(0.2, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-17, 1.1, 1);
	glRotatef(hyot, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glScalef(3, 0.1, 0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.4, 0.1, 3);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();


	glPopMatrix();

	Hzot += 8.0f;
	if (Hzot >= 360.0f)
		Hzot = 0.0f;
	hyot += 5.0f;
	if (hyot >= 360.0f)
		hyot = 0.0f;

	glDisable(GL_COLOR_MATERIAL);
}
void box5()
{
	
	glTranslatef(x5, y11, z1);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslated(-520.0, 0.0, 1.0);
	glRotated(90, 1, 0, 0);
	glRotated(90, 0, 1, 0);
	glScalef(5, 5, 5);
	//画身体
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);
	// 画头
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);
	// 画眼睛
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	// 画鼻子
	glColor3f(1.0f, 0.2f, 0.2f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

void roads()
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, road);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 6.0f, 1.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 6.0f, 1.01f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void road2(int s, int e)
{
	for (int i = s; i >= e; i -= 30)
	{
		glPushMatrix();
		glTranslated(i, 0.0, 0.0);
		roads();
		glPopMatrix();
	}
}

void roads_2()
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, road1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 6.0f, 1.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 6.0f, 1.01f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void road2_2(int s, int e)
{
	for (int i = s; i >= e; i -= 30)
	{
		glPushMatrix();
		glTranslated(i, 0.0, 0.0);
		roads_2();
		glPopMatrix();
	}
}

void roads_3()
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, road22);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 6.0f, 1.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 6.0f, 1.01f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void road2_3(int s, int e)
{
	for (int i = s; i >= e; i -= 30)
	{
		glPushMatrix();
		glTranslated(i, 0.0, 0.0);
		roads_3();
		glPopMatrix();
	}
}

void roads_4()
{
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, road33);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, 6.0f, 1.01f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -6.0f, 1.01f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 6.0f, 1.01f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void road2_4(int s, int e)
{
	for (int i = s; i >= e; i -= 30)
	{
		glPushMatrix();
		glTranslated(i, 0.0, 0.0);
		roads_4();
		glPopMatrix();
	}
}

void B_road()
{
	if (f_r)
	{
		glPushMatrix();
		glTranslatef(rx, ry, rz);
		road2(180, -180);
		rx += v;
		if (rx >= 400)
			f_r = false;
		glPopMatrix();
	}

	if (f_r1)
	{
		glPushMatrix();
		glTranslatef(rx1, ry, rz);
		road2(-210, -570);
		rx1 += v;
		if (rx1 >= 800)
			f_r1 = false;
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(rx2, ry, rz);
	if (ss1)
		road2(-600, -990);
	if (ss2)
		road2_2(-600, -990);
	if (ss3)
		road2_3(-600, -990);
	if (ss4)
		road2_4(-600, -990);
	rx2 += v;
	if (rx2 >= 400)
		f_r2 = true;
	if (rx2 >= 1200)
		rx2 = 0;
	glPopMatrix();

	if (f_r2)
	{
		glPushMatrix();
		glTranslatef(rx3, ry, rz);
		if (ss1)
			road2(-600, -990);
		if (ss2)
			road2_2(-600, -990);
		if (ss3)
			road2_3(-600, -990);
		if (ss4)
			road2_4(-600, -990);
		rx3 += v;
		if (rx3 >= 400)
			f_r3 = true;
		if (rx3 >= 1200)
			rx3 = 0;
		glPopMatrix();
	}

	if (f_r3)
	{
		glPushMatrix();
		glTranslatef(rx4, ry, rz);
		if (ss1)
			road2(-600, -990);
		if (ss2)
			road2_2(-600, -990);
		if (ss3)
			road2_3(-600, -990);
		if (ss4)
			road2_4(-600, -990);
		rx4 += v;
		if (rx4 >= 1200)
			rx4 = 0;
		glPopMatrix();
	}
}


void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str) //屏幕显示字体  
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号  
		lists = glGenLists(128);
		// 把每个字符的绘制命令都装到对应的显示列表中  
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符  
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}

void paint(void)//绘制场景
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.0f, 1.0f, 900.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (s_d)
		gluLookAt(80.0, 30.0, 25.0, 0.0, -20.0, 0.0, 0.0, 0.0, 1.0);
	if (s_a)
		gluLookAt(80.0, -30.0, 25.0, 0.0, 20.0, 0.0, 0.0, 0.0, 1.0);
	if (s_w)
		gluLookAt(80.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	{
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");//字体可以在系统文件里寻找
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0f, 0.0f, 0.0f);//字体颜色
		if (s_d)
			glRasterPos3i(50, -50, 53);//起始位置
		if (s_a)
			glRasterPos3i(-100, -47, 84);//起始位置
		if (s_w)
			glRasterPos3i(0, -60, 54);//起始位置
		char string[25];
		itoa(score, string, 10);
		drawString(string);   //输出的字符串
		glDisable(GL_COLOR_MATERIAL);
	}

	if (!active)
	{
		selectFont(48, ANSI_CHARSET, "Comic Sans MS");//字体可以在系统文件里寻找
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.0f, 0.0f, 0.0f);//字体颜色
		if (s_d)
			glRasterPos3i(60, 10, 20);//起始位置
		if (s_a)
			glRasterPos3i(-90, 10, 0);//起始位置
		if (s_w)
			glRasterPos3i(0, -20, 10);//起始位置
		drawString("GAME OVER");   //输出的字符串
		glDisable(GL_COLOR_MATERIAL);
	}

	if (over)
	{
		for (int i = 0; i<6; i++)
			aactive[i] = false;
		ran_flag = true;
		x0 = 0;
		x1 = 0;
		x2 = 0;
		x3 = 0;
		x4 = 0;
		x5 = 0;
		score = 0;
		over = false;
		ss1 = true;
		ss2 = false;
		ss3 = false;
		ss4 = false;
		s1 = true;
		s2 = true;
		s3 = true;
		rx = 0;
		ry = 0;
		rz = 0;
		rx1 = 0;
		rx2 = 0;
		rx3 = 0;
		rx4 = 0;
		f_r = true;
		f_r1 = true;
		f_r2 = false;
		f_r3 = false;
		aa = 2.4;
		bb = 1.6;
		v = 0.45;
		s_w = false;
		s_a = false;
		s_d = true;
	}


	if (active)
	{
		glPushMatrix();
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		{
			glEnable(GL_COLOR_MATERIAL);
			glColor4d(1, 1, 1, 1);
			int tmp1 = 400, tmp2 = -520, tmp3 = 280;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, front);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp2, -tmp1, -tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp2, -tmp1, tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp2, tmp1, tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp2, tmp1, -tmp1);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, back);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp3, -tmp1, -tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp3, -tmp1, tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp3, tmp1, tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp3, tmp1, -tmp1);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, lf);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp3, -tmp1, -tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp3, -tmp1, tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp2, -tmp1, tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp2, -tmp1, -tmp1);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, rt);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp3, tmp1, -tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp3, tmp1, tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp2, tmp1, tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp2, tmp1, -tmp1);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, up);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp3, -tmp1, tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp2, -tmp1, tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp2, tmp1, tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp3, tmp1, tmp1);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, dn);
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);glVertex3f(tmp3, -tmp1, -tmp1);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(tmp2, -tmp1, -tmp1);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(tmp2, tmp1, -tmp1);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(tmp3, tmp1, -tmp1);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_COLOR_MATERIAL);
		}glPopMatrix();
		

		if (ran_flag)
		{
			srand((unsigned)time(NULL));
			int ran1 = rand() % 6;
			while (aactive[ran1])
				ran1 = rand() % 6;
			ran = ran1;
			ran_flag = false;
			aactive[ran] = true;
		}

		if (true)
		{
			glPushMatrix();
			box0();
			if (aactive[0])
			{
				if (x0 <= 600)
				{
					x0 += v;
					if (fabs(x0 - 120.0f) <= v/2)
						ran_flag = true;
					if (x0 > 600)
					{
						aactive[0] = false;
						x0 = 0;
						score++;
					}
				}
				if (x0 >= 575 && x0 <= 590 && !flag)
					active = false;
			}
			glPopMatrix();
		}
		if (true)
		{
			glPushMatrix();
			box1();
			if (aactive[1])
			{
				if (x1 <= 600)
				{
					x1 += v;
					if (fabs(x1 - 180.0f) <= v / 2)
						ran_flag = true;
					if (x1 > 600)
					{
						aactive[1] = false;
						x1 = 0;
						score++;
					}

				}
				if (x1 >= 575 && x1 <= 592 && !flag)
					active = false;
			}

			glPopMatrix();
		}
		if (aactive[2])
		{
			glPushMatrix();
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			box2();
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_COLOR_MATERIAL);
			if (x2 <= 600)
			{
				x2 += v;
				if (fabs(x2 - 100.0f) <= v / 2)
					ran_flag = true;
				if (x2 > 600)
				{
					aactive[2] = false;
					x2 = 0;
					score++;
				}

			}
			if (x2 >= 577 && x2 <= 590 && (flag || (!down && !flag)))
				active = false;
			glPopMatrix();
		}
		if (true)
		{
			glPushMatrix();
			box3();
			if (aactive[3])
			{
				if (x3 <= 600)
				{
					x3 += v;
					if (fabs(x3 - 130.0f) <= v / 2)
						ran_flag = true;
					if (x3 > 600)
					{
						aactive[3] = false;
						x3 = 0;
						score++;
					}

				}
				if (x3 >= 575 && x3 <= 590 && !flag)
					active = false;
			}
			glPopMatrix();
		}
		if (true)
		{
			glPushMatrix();
			box4();
			if (aactive[4])
			{
				if (x4 <= 600)
				{
					x4 += v;
					if (fabs(x4 - 150.0f) <= v / 2)
						ran_flag = true;
					if (x4 > 600)
					{
						aactive[4] = false;
						x4 = 0;
						score++;
					}

				}
				if (x4 >= 573 && x4 <= 593 && (flag || (!down && !flag)))
					active = false;
			}
			glPopMatrix();
		}
		if (true)
		{
			glPushMatrix();
			box5();
			if (aactive[5])
			{
				if (x5 <= 600)
				{
					x5 += v;
					if (fabs(x5 - 150.0f) <= v / 2)
						ran_flag = true;
					if (x5 > 600)
					{
						aactive[5] = false;
						x5 = 0;
						score++;
					}

				}
				if (x5 >= 573 && x5 <= 593 && !flag)
					active = false;
			}
			glPopMatrix();
		}



		//地板
		{
			glPushMatrix();
			glEnable(GL_COLOR_MATERIAL);
			glTranslated(-120.0, 0.0, 0.0);

			B_road();

			if (score >= 20 && score < 40)
			{
				ss1 = false;
				ss2 = true;
				ss3 = false;
				ss4 = false;

				glColor3f(1, 1, 0);
				if (s1)
				{
					aa += 1.5;
					bb += 1.2;
					v += 0.1;
					s1 = false;
				}
			}

			else if (score >= 40 && score < 60)
			{
				ss1 = false;
				ss2 = false;
				ss3 = true;
				ss4 = false;
				glColor3f(1, 0, 1);
				if (s2)
				{
					aa += 1.5;
					bb += 1.2;
					v += 0.1;
					s2 = false;
				}
			}

			else if (score >= 60)
			{
				ss1 = false;
				ss2 = false;
				ss3 = false;
				ss4 = true;
				glColor3f(0, 1, 1);
				if (s3)
				{
					aa += 1.5;
					bb += 1.2;
					v += 0.1;
					s3 = false;
				}
			}

			else
				glColor3f(1, 0, 0);
			glScalef(800, 12, 2);
			glutSolidCube(1);


			glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
		}


		//机器人
		{
			glPushMatrix();
			glTranslatef(rox, roy, roz);
			glEnable(GL_COLOR_MATERIAL);
			glRotated(90, 1.0, 0.0, 0.0);
			glRotated(-90, 0.0, 1.0, 0.0);
			glTranslated(39.9, 10, 35.0);
			glScalef(2, 2, 2.5);
			if (!down)
				DrawRobot();
			else
			{
				DrawDownRobot();
				down_num++;
				if (down_num == 150)
				{
					down = false;
					down_num = 0;
				}

			}

			if (flag)
			{
				if (roz < 15.0 && !jump)
				{
					roz += high;
					high -= 0.001;
				}

				if ((roz >= 15.0 || jump) && roz > 0)
				{
					roz -= high;
					roz = max(roz, 0.0f);
					high += 0.001;
					jump = true;
				}
				if (roz <= 0.000001 && jump)
				{
					jump = false;
					flag = false;
					high = 0.3;
				}
			}

			glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
		}


		glPopMatrix();

	}
	glutSwapBuffers();
	glutPostRedisplay();
}
void Init()
{
	makeCubeMap();
	initRobot();
	LoadGLTextures(bas, "bas.bmp");
	LoadGLTextures(mc, "mc.bmp");
	LoadGLTextures(road, "road.bmp");
	LoadGLTextures(road1, "road1.bmp");
	LoadGLTextures(road22, "road2.bmp");
	LoadGLTextures(road33, "road3.bmp");
	LoadGLTextures(back, "sincity_bk.bmp");
	LoadGLTextures(dn, "sincity_dn.bmp");
	LoadGLTextures(front, "sincity_ft.bmp");
	LoadGLTextures(lf, "sincity_lf.bmp");
	LoadGLTextures(rt, "sincity_rt.bmp");
	LoadGLTextures(up, "sincity_up.bmp");
	//灯光
	{
		GLfloat sun_light_position[] = { 250.0f, 0.0f, 60.0f, 1.0f };
		GLfloat sun_light_ambient[] = { 0.5f,  0.5f,  0.5f,  1.0f };   //环境光
		GLfloat sun_light_diffuse[] = { 1.0f,  1.0f,  1.0f,  1.0f };   //漫反射
		GLfloat sun_light_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };    //镜面光

		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		GLfloat light1_position[] = { 50.0f, 50.0f, 10.0f, 1.0f };
		GLfloat light1_ambient[] = { 0.5f,  0.5f,  0.5f,  1.0f };   //环境光
		GLfloat light1_diffuse[] = { 0.5f,  0.5f,  0.5f,  1.0f };   //漫反射
		GLfloat light1_specular[] = { 0.5f,  0.5f,  0.5f,  1.0f };    //镜面光

		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
		glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  //指定材料着色的面
		glMateriali(GL_FRONT, GL_SHININESS, 80);            //指定反射系数
	}
	glEnable(GL_FOG);
	{
		GLfloat fogColor[4] = {0.7,0.7,0.7,1.0};
		fogMode = GL_EXP2;
		glFogi(GL_FOG_MODE, fogMode);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, 0.003);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 1.0);
		glFogf(GL_FOG_END, 5.0);
	}
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(2.0);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void mouseMove(int x, int y)//鼠标移动后的操作
{
	if (xOrigin >= 0 && yOrigin >= 0)
	{

		deltaAnglex = (x - xOrigin) * 0.015f;
		deltaAngley = (y - yOrigin) * 0.015f;

		zRot += deltaAnglex;
		xRot -= deltaAngley;
		if (zRot > 360.0f)
			zRot = 0.0f;

		if (zRot< 0.0f)
			zRot = 360.0f;

		if (xRot > 360.0f)
			xRot = 0.0f;

		if (xRot< 0.0f)
			xRot = 360.0f;

		glutPostRedisplay();
	}
}
void mouseButton(int button, int state, int x, int y)//鼠标点击后的操作
{

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP) {
			deltaAnglex = 0.0f;
			deltaAngley = 0.0f;
			xOrigin = -1;
			yOrigin = -1;
		}
		else {
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void SpecialKeys(int key, int x, int y)//键盘操作
{
	if (key == GLUT_KEY_F1)
	{
		if (!active && !over)
			over = !over;
		if (!active)
			active = !active;
	}
	if (key == GLUT_KEY_UP)
	{
		if (!flag && !down)
			flag = true;
	}
	if (key == GLUT_KEY_DOWN)
	{
		if (!down && !flag)
			down = true;
	}

	glutPostRedisplay();
}

void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		if (active)
		{
			s_d = false;
			s_a = false;
			s_w = true;
		}

		break;
	case 'a':
		if (active)
		{
			s_d = false;
			s_a = true;
			s_w = false;
		}
		break;
	case 'd':
		if (active)
		{
			s_d = true;
			s_a = false;
			s_w = false;
		}

	}
}

void myIdle()
{
	glutPostRedisplay();
}

int main(int argv, char *argc[])
{
	glutInit(&argv, argc);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(700, 600);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Running");
	printf("F1:    重新开始比赛\n\n");
	printf("↑:    上跳\n\n");
	printf("↓:    滑行\n\n");
	printf("视角:\n\n");
	printf("s:    后方\n\n");
	printf("a:    左侧\n\n");
	printf("d:    右侧");
	Init();
	glutDisplayFunc(paint);
	//glutIdleFunc(myIdle);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(keyBoard);
	//glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseMove);
	glutMainLoop();
}
