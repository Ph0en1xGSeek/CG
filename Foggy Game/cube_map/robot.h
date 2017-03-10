#include <iostream>
#include <Windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL\glext.h>
#include "texture.h"
using namespace std;

static float angleoflarma,angleoflarmb,angleofrarma,angleofrarmb,angleoflleg,angleofrleg, angleoflleg1, angleofrleg1, aa = 2.4,bb = 1.6, a = 2.4, b = 1.6, rox = 0,roy = 0,roz = 0;
extern bool flag;


unsigned int LoadTex(char* Image);
int LoadGLTextures(GLuint& unTexture, char* chFileName);
void initRobot();
void DrawBody();
void DrawShoulder();
void DrawWaise();
void DrawArmA();
void DrawArmB();
void DrawLeg();
void DrawHead();
void DrawNick();
void DrawRobot();
void DrawDownRobot();
