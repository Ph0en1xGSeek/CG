#include <iostream>
#include <Windows.h>
#include <GL\freeglut.h>
using namespace std;


static float angleoflarma,angleoflarmb,angleofrarma,angleofrarmb,angleoflleg,angleofrleg,a = 1.0,b=0.5,angle = 90.0;

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
