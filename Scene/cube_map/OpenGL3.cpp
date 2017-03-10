#include<stdio.h>
#include <iostream>
#include <time.h>
#include <assert.h>
#include<math.h>
#include <Windows.h>
#include <GL\glew.h>
#include<gl/freeglut.h>
#include "robot.h"
#include <GL/glext.h>
#include "texture.h"
#include "cars.h"
#include <conio.h>
#include "tga.h"		/* simple TGA image file loader */
#include "trackball.h"	/* trackball quaternion routines */
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
HANDLE handle0 = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;
///--------------------------------------------------模型-----------------------------------------------
GLuint drawcube, drawsphere, drawteapot, snowman;
///---------------------------------------------------贴图-
GLuint ground, blue, red, earth, plane, moon, teapot, book ,wood, purple, wood2, stone, jupiter;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB = NULL;
///----------------------------------------------------动画、漫游-----------
int clickx, clicky;
bool isMouseDown = false;
static float r_angle = 1.0;
static float s_angle = 30.0;
//static GLfloat sx = 0, sy = 0, sz = 0;
static GLfloat s_eye[] = { -62.0f, 8.0f, -32.0f };
static GLfloat s_at[] = { 0, 8.0f, 29.6f };
float speed = 0.5f;
float PI = acos(-1);
static int lflag = GL_TRUE, rflag = GL_TRUE;
bool flag = false;
int winW, winH;
int rt = 0, cnt = 0, direction = false, upcnt = 0, updirection = false;
int pos[200][3];
int ticker = 0;
///-------------------------------------------------------材质-----------------------------------------
float no_mat[] = {0.0,0.0,0.0,1.0};
float mat_red[] = {0.8,0.1,0.0,1.0};
float mat_ambient[] = {0.3,0.3,0.3,1.0};
float mat_orange[] = {1.0,0.2,0.0,1.0};
float mat_diffuse[] = {0.5,0.5,0.5,1.0};
float mat_specular[] = {1.0,1.0,1.0,1.0};
float no_shininess[] = {0.0};
float low_shininess[] = {5.0};
float high_shininess[] = {100.0};
float mat_emmision[] = {0.3,0.2,0.3,0.0};
///-----------------------------------------------光照------------------
bool light_on = false;
bool lamp_on = false;
///============================================

///=========================================Cube Mapping========================================
bool mipmaps = true;
int mode = GL_REFLECTION_MAP;
int wrap = GL_CLAMP_TO_EDGE;

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
	"cm_left.tga",
	"cm_right.tga",
	"cm_top.tga",
	"cm_bottom.tga",
	"cm_back.tga",
	"cm_front.tga",
};
void updateTexgen(void)
{
	assert(mode == GL_NORMAL_MAP || mode == GL_REFLECTION_MAP);
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

///============================================================================================
struct Point
{
    double x, y, z;
}P1[4], P2[4], B1[402], B2[402];
int C[4] = {1,3,3,1};


void reshape(int w, int h)
{
	winW = w, winH = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (flag == false)
		gluPerspective(90, w / h, 0.5, 700);
	else
		glOrtho(-50, 50, -50, 50, 0, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	makeCubeMap();
    initRobot();
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH, GL_FASTEST);
    glEnable(GL_TEXTURE_2D);
    LoadGLTextures(ground, "ground.bmp");
    LoadGLTextures(blue, "blue.bmp");
    LoadGLTextures(red, "red.bmp");
    LoadGLTextures(purple, "purple.bmp");
    LoadGLTextures(earth, "earth.bmp");
    LoadGLTextures(plane, "plane.bmp");
    LoadGLTextures(moon, "moon.bmp");
    LoadGLTextures(teapot, "teapot.bmp");
    LoadGLTextures(book, "book.bmp");
    LoadGLTextures(wood, "wood.bmp");
    LoadGLTextures(wood2, "wood2.bmp");
    LoadGLTextures(stone, "stone.bmp");
    LoadGLTextures(jupiter, "muxing.bmp");


    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	srand(time(0));
	for (int i = 0; i < 200; i++)
	{
		pos[i][0] = rand() % 600 - 300;
		pos[i][1] = rand() % 400;
		pos[i][2] = rand() % 600 - 300;
	}
	//画出正方体显示列表
	drawcube = glGenLists(1);
	glNewList(drawcube, GL_COMPILE);
	glBegin(GL_QUADS);
    glNormal3d(0,1,0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0.5f, 0.5f, -0.5f);
    glNormal3d(0,-1,0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0.5f, -0.5f, -0.5f);
    glNormal3d(1,0,0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0.5f, -0.5f, 0.5f);
    glNormal3d(-1,0,0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5, 0.5f, 0.5);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.5, 0.5f, -0.5);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.5, -0.5f, -0.5);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.5, -0.5f, 0.5);
    glNormal3d(0,0,1);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0.5, 0.5f, 0.5);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.5, 0.5f, 0.5);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5, -0.5f, 0.5);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0.5, -0.5f, 0.5);
    glNormal3d(0,0,-1);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0.5, 0.5f, -0.5);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.5, 0.5f, -0.5);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5, -0.5f, -0.5);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(0.5, -0.5f, -0.5);
    glEnd();
	glEndList();
	//画出球体显示列表
	drawsphere = glGenLists(1);
	glNewList(drawsphere, GL_COMPILE);
    GLUquadricObj *qobject = gluNewQuadric();
    gluQuadricDrawStyle(qobject, GLU_FILL);
    gluQuadricNormals(qobject, GLU_SMOOTH);
    gluQuadricTexture(qobject, GL_TRUE);
    gluSphere(qobject, 25.0, 40, 40);
	glEndList();
	//画出茶壶显示列表
	drawteapot = glGenLists(1);
	glNewList(drawteapot, GL_COMPILE);
	glutSolidTeapot(1);
	glEndList();

	snowman = glGenLists(1);
	glNewList(snowman, GL_COMPILE);
	glScalef(10, 10, 10);
	//画身体
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_diffuse);
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
	glEndList();

	//设置普通灯光照0位置及参数；
	GLfloat position0[] = { 0,250,0,-1};
	GLfloat light0s[] = { 0.3f,0.3f,0.3f,0.3f };
	GLfloat light0d[] = { 0.3f,0.3f,0.3f, 0.3f};
	GLfloat light0a[] = { 0.7f,0.7f,0.7f, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0s);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0d);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0a);

    GLfloat position2[] = { 20, 0,10,-1};
	GLfloat light2s[] = { 1,0.8,0,1.0};
	GLfloat light2d[] = { 1,0.8,0,1.0};
	GLfloat light2a[] = { 0.5f,0.1f,0.0f,1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2s);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2d);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2a);

	GLfloat  direction[] = { 200,-200,0};
	//设置探照灯光照1位置及参数
	GLfloat position1[] = { -200, 200,0,1};
	GLfloat light1s[] = { 0.5,0.5,0.5,0.5 };
	GLfloat light1d[] = { 0.5f,0.5f,0.5f,0.5 };
	GLfloat light1a[] = { 0.50f,0.5f,0.5f,1.0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1s);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1d);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1a);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	///-----------------------------------------------------------------------------

	P1[0].x = 260.2, P1[0].y = 190.2, P1[0].z = 210.2;
	P1[1].x = 247.2, P1[1].y = 202.2, P1[1].z = -264.2;
	P1[2].x = -125.2, P1[2].y = 110.2, P1[2].z = 150.2;
	P1[3].x = -200.2, P1[3].y = 160.2, P1[3].z = -190.2;

	P2[0].x = -200.2, P2[0].y = 160.2, P2[0].z = -190.2;
	P2[1].x = 140.2, P2[1].y = 122.2, P2[1].z = -113.2;
	P2[2].x = -167.2, P2[2].y = 120.2, P2[2].z = 150.2;
	P2[3].x = 260.2, P2[3].y = 190.2, P2[3].z = 210.2;

	for(int t = 0; t <= 400; t++)
    {
        B1[t].x = 0, B1[t].y = 0, B1[t].z = 0;
        for(int i = 0; i < 4; i++)
        {
            B1[t].x += C[i] * P1[i].x * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
            B1[t].y += C[i] * P1[i].y * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
            B1[t].z += C[i] * P1[i].z * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
        }
    }

    for(int t = 0; t <= 400; t++)
    {
        B2[t].x = 0, B2[t].y = 0, B2[t].z = 0;
        for(int i = 0; i < 4; i++)
        {
            B2[t].x += C[i] * P2[i].x * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
            B2[t].y += C[i] * P2[i].y * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
            B2[t].z += C[i] * P2[i].z * pow(1-t/400.0, 3-i) * pow(t/400.0, i);
        }
    }

}


void AirRoam(void)
{
	if (flag == false)
	{
		if (KEY_DOWN(VK_LEFT))
		{
			s_angle -= 1.0;
		}
		if (KEY_DOWN(VK_RIGHT))
		{
			s_angle += 1.0;
		}
		float rad = float(PI*s_angle / 180.0f);
		float beta = float(PI*r_angle / 180.0f);
		// 前进,后退请求
		if (KEY_DOWN(VK_UP))
		{
			s_eye[2] += (float)sin(rad) * cos(beta) * speed;
			s_eye[1] += (float)sin(beta) * speed;
			s_eye[0] += (float)cos(rad) * cos(beta) * speed;
		}
		if (KEY_DOWN(VK_DOWN))
		{
			s_eye[2] -= (float)sin(rad) * cos(beta) *speed;
			s_eye[1] -= (float)sin(beta) * speed;
			s_eye[0] -= (float)cos(rad) * cos(beta) * speed;
		}

		// 观察点
		s_at[0] = float(s_eye[0] + 100.0 * cos(rad) * cos(beta));
		s_at[2] = float(s_eye[2] + 100.0 * sin(rad) * cos(beta));
		s_at[1] = float(s_eye[1] + 100.0 * sin(beta));
		float b;
		if (fabs(s_at[1] - s_eye[1]) < 1e-4)
			b = 1.0f;
		else
			b = (((s_eye[0] - s_at[0])*(s_eye[0] - s_at[0]) + (s_eye[2] - s_at[2])*(s_eye[2] - s_at[2])) / fabs(s_at[1] - s_eye[1]));
		//设置观察点
		glLoadIdentity();
		gluLookAt(s_eye[0], s_eye[1], s_eye[2],//从哪
			s_at[0], s_at[1], s_at[2],//看向哪
			s_eye[0] - s_at[0], b, s_eye[2] - s_at[2]);//头向上
	}
	else
	{

	}
	// 旋转请求

	glutPostRedisplay();
}

void DrawGroud(int tmp)
{
	glPushMatrix();
	glPushMatrix();
	draw_car(2,6);
	glPopMatrix();
	glPushMatrix();
	glTranslated(20,-45,-35);
	glRotated(-90,1,0,0);
	glRotated(180, 0,0,1);
	glScaled(0.1,0.1,0.1);
	create_lamp(50,0,50,700);
	glPopMatrix();
	glPushMatrix();
	glColor4d(1,1,1,0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, moon);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);glVertex3f(300.0f, -40.0f, 300.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-300.0f, -40.0f, 300.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-300.0f, -40.0f, -300.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(300.0f, -40.0f, -300.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, ground);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(300.0f, 400.0f, 300.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-300.0f, 400.0f, 300.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-300.0f, 400.0f, -300.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(300.0f, 400.0f, -300.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, red);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(300.0f, 300.0f, 300.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(300.0f, -300.0f, 300.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-300.0f, -300.0f, 300.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-300.0f, 300.0f, 300.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, earth);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(300.0f, -300.0f, -300.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(300.0f, 300.0f, -300.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-300.0f, 300.0f, -300.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-300.0f, -300.0f, -300.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, plane);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(300.0f, -300.0f, 300.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(300.0f, 300.0f, 300.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(300.0f, 300.0f, -300.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(300.0f, -300.0f, -300.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    ///----------------------------------------------多重纹理------------------------------------------
    glPushMatrix();
    glEnable(GL_BLEND);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wood2);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, purple);

    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
    glVertex3f(-300.0f, 300.0f, -300.0f);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
    glVertex3f(-300.0f, 300.0f, 300.0f);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
    glVertex3f(-300.0f, -300.0f, 300.0f);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
    glVertex3f(-300.0f, -300.0f, -300.0f);
    glEnd();
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
///----------------------------------------------------------------------------------------------
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	srand(rand());
	for (int i = 0; i < 200; i+=2)
	{
		glPointSize((rand()%9)/3);
		glPushMatrix();
		glBegin(GL_POINTS);
		glVertex3f(pos[i][0], pos[i][1], pos[i][2]);
		glEnd();
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(-80, 10, -200);

	glColor3f(0.4, 0.4, 0.4);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glBegin(GL_LINE_STRIP);          //北斗七星
	glVertex3f(289, 190,0);
	glVertex3f(320, 128,0);
	glVertex3f(239, 67,0);
	glVertex3f(194, 101,0);
	glVertex3f(129, 88,0);
	glVertex3f(75, 73,0);
	glVertex3f(20, 10,0);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(289, 190, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(320, 128, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(239, 67, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(194, 101, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(129, 88, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(75, 73, 0);
	glPointSize((rand() % 9) / 2.5);
	glVertex3f(20, 10, 0);
	glEnd();
	glPopMatrix();

	glTranslatef(0, -20, 0);
	glPushMatrix();
	glTranslatef(0, -20, 0);
	glColor3f(0.2, 0.2, 0.2);

	glPopMatrix();
	//绘制四个桌腿：
	glPushMatrix();
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glTranslatef(20, -10, -20);
	glScalef(1, 20, 1);//桌脚底会下降10 桌脚顶会上升10
	//glCallList(drawcube);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood2);
    glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-20, -10, -20);
	glScalef(1, 20, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood2);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-20, -10, 20);
	glScalef(1, 20, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood2);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(20, -10, 20);
	glScalef(1, 20, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood2);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	///画出桌面：
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glScalef(50,1,50);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wood);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);
	// glFlush();

	glPopMatrix();
	///绘制两边台阶；
	glPushMatrix();//左边第一级台阶
    glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glTranslatef(-30, -16, 0);
	glScalef(10, 4, 50);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stone);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();// 右边第一级台阶

	glTranslatef(40, -16, 0);
	glScalef(10, 4, 50);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stone);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//左边第二级台阶

	glTranslatef(-32.5, -12, 0);
	glScalef(5, 4, 50);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stone);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();//右边第二级台阶

	glTranslatef(42.5, -12, 0);
	glScalef(5, 4, 50);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stone);
	glCallList(drawcube);
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);
///=======================================================================================
	glDisable(GL_COLOR_MATERIAL);
	glPushMatrix();///茶壶
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0, 6, 0);
	glScalef(5, 8, 5);
	glRotatef(45, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, teapot);
	glCallList(drawteapot);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glColor4f(1, 1, 1, 1);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glMateriali(GL_FRONT, GL_SHININESS, 100);///金属材质
	GLUquadricObj *pObj;
	glPushMatrix();//第一个杯子  左边
	pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);
	glTranslatef(-8, 6, 8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(pObj, 1.50f, 1.0f, 5.0f, 26, 13);
	glPopMatrix();
	glPushMatrix();  //第2个杯子 右边
	glTranslatef(8, 6, 8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(pObj, 1.50f, 1.0f, 5.0f, 26, 13);
    glDisable(GL_BLEND);
	glPopMatrix();
	glPushMatrix();  //桌面上垫圈

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0.21, 0.21, 0.21, 1);
	pObj = gluNewQuadric();
	gluQuadricDrawStyle(pObj,GLU_LINE);
	glTranslatef(-8, 1, 8);
	glRotatef(90, 1, 0, 0);
	gluDisk(pObj, 0.50f, 3.0f, 10, 10);
	glPopMatrix();
	glPushMatrix();  //桌面上垫圈

	pObj = gluNewQuadric();
	glTranslatef(8, 1, 8);
	glRotatef(90, 1, 0, 0);
	gluDisk(pObj, 0.50f, 3.0f, 30, 30);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_COLOR_MATERIAL);
//	glPushMatrix();
//	GLfloat data[] = {	0.4f, 0.3f, 0.1f,-5.0f,-5.0f,-5.0f,
//						0.3f, 0.9f, 0.3f,-5.0f,-5.0f,5.0f,
//						0.2f, 0.2f, 0.3f,-5.0f,5.0f,5.0f,
//						0.2f, 0.4f, 0.6f,-5.0f,5.0f,-5.0f,
//						0.4f, 0.7f, 0.5f,5.0f,-5.0f,-5.0f,
//						1.0f, 0.3f, 0.6f,5.0f,-5.0f,5.0f,
//						0.2f, 0.0f, 0.8f,5.0f,5.0f,5.0f,
//						0.9f, 0.0f, 0.8f,5.0f,5.0f,-5.0f};
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &data[0]);
//	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &data[3]);
//	glTranslatef(50, 70, -50);//要先平移
//	glScalef(2, 2, 2);
//	//glRotatef(45, 1, 1, 1);
//	glRotatef(-rt, 0, 1, 0);
//	glBegin(GL_QUADS);
//	glArrayElement(0);
//	glArrayElement(1);
//	glArrayElement(2);
//	glArrayElement(3);
//
//	glArrayElement(4);
//	glArrayElement(5);
//	glArrayElement(6);
//	glArrayElement(7);
//
//	glArrayElement(0);
//	glArrayElement(1);
//	glArrayElement(5);
//	glArrayElement(4);
//
//	glArrayElement(2);
//	glArrayElement(3);
//	glArrayElement(7);
//	glArrayElement(6);
//
//	glArrayElement(0);
//	glArrayElement(3);
//	glArrayElement(7);
//	glArrayElement(4);
//
//	glArrayElement(1);
//	glArrayElement(2);
//	glArrayElement(6);
//	glArrayElement(5);
//	glEnd();
//	glPopMatrix();
    glColor3f(0.1,0.1,0.1);
	glPushMatrix();///木星系

	glBegin(GL_LINE_STRIP);          ///贝塞尔
    for(int t = 0; t <= 400; t++)
    {
        glVertex3f(B1[t].x, B1[t].y, B1[t].z);
    }
    for(int t = 0; t <= 400; t++)
    {

        glVertex3f(B2[t].x, B2[t].y, B2[t].z);
    }
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_orange);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_orange);
	if(ticker < 400)
        glTranslatef(B1[ticker].x, B1[ticker].y, B1[ticker].z);
	else
        glTranslatef(B2[ticker-400].x, B2[ticker-400].y, B2[ticker-400].z);
	glRotatef(rt, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupiter);
	glCallList(drawsphere);
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glRotatef(rt, 0.0, 1.0, 0.0);
	glTranslatef(40.0, 0.0, 0.0);
	glColor3f(0.0, 0.2, 0.8);
	glutWireSphere(1.3, 10, 8);
	glPushMatrix();
	glRotatef(rt*3, -1.0, 1.0, 0.0);
	glTranslatef(2.0, 2.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutWireSphere(0.4, 10, 8);
	glPopMatrix();//卫星
	glPopMatrix();//行星
	glPushMatrix();
    glRotatef(rt/2, 0.0, 1.0, 0.0);
	glTranslatef(50.0, 0.0, 0.0);
	glColor3f(0.8, 0.2, 0.0);
	glutWireSphere(1.6, 10, 8);
	glPopMatrix();
    glPushMatrix();
    glRotatef(rt*2, 0.0, 1.0, 0.0);
	glTranslatef(30.0, 0.0, 0.0);
	glColor3f(0.2, 0.4, 0.1);
	glutWireSphere(1.2, 10, 8);
	glPopMatrix();
    glPushMatrix();
    glRotatef(rt/3, 0.0, 1.0, 0.0);
	glTranslatef(45.0, 0.0, 0.0);
	glColor3f(0.3, 0.2, 0.6);
	glutWireSphere(1.0, 10, 8);
	glPopMatrix();
	glPopMatrix();//恒星

	glPushMatrix();//雪人
	glPushMatrix();
	glDisable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glMateriali(GL_FRONT, GL_SHININESS, 5);///高光材质
	glColor3f(0.8f, 0.8f, 0.8f);
	glTranslatef(0, -20, -50);
	if (tmp <= -20)
		glRotatef(-90, 0, 1, 0);
	glCallList(snowman);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.8f);
	glTranslatef(0, -20, -35);
	if(tmp <= -20)
		glRotatef(-90, 0, 1, 0);
	else
		glRotatef(180, 0, 1, 0);
	glCallList(snowman);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, -20 + upcnt/10,tmp);
	glColor3f(0.8f, 0.8f, 0.8f);
	if(direction)
		glRotatef(180, 0, 1, 0);
	glCallList(snowman);
	glPopMatrix();
	glEnable(GL_COLOR_MATERIAL);
	///===========================机器人====================
    glPushMatrix();
        glScalef(8,5,5);
        glTranslatef(0,3,40);
        DrawRobot();
    glPopMatrix();
    ///================================================
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, book);
    glDisable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    //glColor4f(1,1,1,1);
    glTranslated(0,-18,-15);
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-6.5f, 0, 5.0f);///红宝书
    glTexCoord2f(0.0f, 0.0f);glVertex3f(6.5f, 0, 5.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(6.5f, 0, -5.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-6.5f, 0, -5.0f);

    glColor4f(0.5,0,0,1);
    glNormal3f(0,-1,0);
    glVertex3f(-6.5f, -1, 5.0f);
    glVertex3f(6.5f, -1, 5.0f);
    glVertex3f(6.5f, -1, -5.0f);
    glVertex3f(-6.5f, -1, -5.0f);

    glNormal3f(0,0,1);
    glVertex3f(-6.5f, -1, 5.0f);
    glVertex3f(6.5f, -1, 5.0f);
    glVertex3f(6.5f, 0, 5.0f);
    glVertex3f(-6.5f, 0, 5.0f);

    glColor4f(1,1,1,1);
    glNormal3f(0,0,-1);
    glVertex3f(-6.5f, -1, -5.0f);
    glVertex3f(6.5f, -1, -5.0f);
    glVertex3f(6.5f, 0, -5.0f);
    glVertex3f(-6.5f, 0, -5.0f);

    glNormal3f(1,0,0);
    glVertex3f(6.5f, -1, 5.0f);
    glVertex3f(6.5f, -1, -5.0f);
    glVertex3f(6.5f, 0, -5.0f);
    glVertex3f(6.5f, 0, 5.0f);

    glNormal3f(-1,0,0);
    glVertex3f(-6.5f, -1, 5.0f);
    glVertex3f(-6.5f, -1, -5.0f);
    glVertex3f(-6.5f, 0, -5.0f);
    glVertex3f(-6.5f, 0, 5.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
}
int slowDown = 1;
void draw()
{
    if(slowDown == 2)
    {
        rt++;
        rt %= 1080;
        slowDown = 1;
        ticker++;
        ticker %= 800;
        if (!direction)
        {
            cnt++;
            if (cnt == 480)
                direction = true;
        }
        else
        {
            cnt--;
            if (cnt == 0)
                direction = false;
        }
        if (!updirection)
        {
            upcnt++;
            if (upcnt == 40)
                updirection = true;
        }
        else
        {
            upcnt--;
            if (upcnt == 0)
                updirection = false;
        }
    }
    else
        slowDown++;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//不加这一句会重影
    DrawGroud(cnt/4-60);
    AirRoam();
    glutSwapBuffers();
}

void mouse(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isMouseDown = true;
		clickx = x;
		clicky = y;
	}
	else if (key == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isMouseDown = false;
	}
}

void motion(int x, int y)
{
	if (isMouseDown)
	{
		if (y > clicky)
		{
			r_angle -= (clicky - y)/8.0;
			r_angle = min(60.0f, r_angle);
			clicky = y;
			//printf("r_ang %lf\n", r_angle);
			//printf("up\n");
		}
		else if (y < clicky)
		{
			r_angle += (y - clicky) /8.0;
			r_angle = max(-44.0f, r_angle);
			clicky = y;
			//printf("r_ang %lf\n", r_angle);
			//printf("down\n");
		}

		if (x < clickx)
		{
			s_angle += (clickx - x) / 8.0;
			clickx = x;
			//printf("s_ang %lf\n", s_angle);
			//printf("up\n");
		}
		else if (x > clickx)
		{
			s_angle -= (x - clickx) / 8.0;
			clickx = x;
			//printf("s_ang %lf\n", s_angle);
			//printf("down\n");
		}
	}
}

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_HOME)
	{
		glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-50, 50, -50, 50, 0, 50);
		flag = true;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else if (key == GLUT_KEY_END)
	{
		glViewport(0, 0, (GLsizei)winW, (GLsizei)winH);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, winW / winH, 0.5, 600);
		flag = false;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'o':
		    for(int i = 0; i < 21; i++)
                printf("\b");
		    if(!light_on)
            {
                light_on = true;
                glEnable(GL_LIGHT1);
            }
            else
            {
                light_on = false;
                glDisable(GL_LIGHT1);
            }
            printf("聚光灯：");
            if(light_on)
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                printf("开   ");
            }
            else
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("关   ");
            }
            SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("路灯：");
            if(lamp_on)
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                printf("开");
            }
            else
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("关");
            }
            SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case 'p':
		    for(int i = 0; i < 21; i++)
                printf("\b");
            if(!lamp_on)
            {
                lamp_on = true;
                glEnable(GL_LIGHT2);
            }
            else
            {
                lamp_on = false;
                glDisable(GL_LIGHT2);
            }
            printf("聚光灯：");
            if(light_on)
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                printf("开   ");
            }
            else
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("关   ");
            }
            SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("路灯：");
            if(lamp_on)
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                printf("开");
            }
            else
            {
                SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("关");
            }
            SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[])
{
	glewInit();
    SetConsoleTextAttribute(handle0, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("↑：向前走\n");
    printf("↓：向后退\n");
    printf("←：向左转\n");
    printf("→：向右转\n");
    printf("鼠标拖动：改变视角\n");
    printf("O：开/关聚光灯\n");
    printf("P：开/关路灯\n\n\n\n");
    printf("环境光默认开\n");
    printf("聚光灯：关   路灯：关");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scene");
	init();
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(draw);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyBoard);
	glutMainLoop();
    return 0;
}
