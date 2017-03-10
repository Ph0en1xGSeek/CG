#include "robot.h"
using namespace std;

GLuint face, cloth, pants;
bool flag = false;


void initRobot()
{
    LoadGLTextures(face, "face.bmp");
    LoadGLTextures(cloth, "cloth.bmp");
	LoadGLTextures(pants, "pants.bmp");
}
void DrawBody(){
  glColor4f(1.0f,1.0f,1.0f,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cloth);
  glBegin(GL_QUADS);
  //forward

    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.5f, 1.0f, 0.25f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f( 0.5f, 1.0f, 0.25f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f( 0.5f,-1.0f, 0.25f);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5f,-1.0f, 0.25f);
  //left
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f, 0.25f);
  //back
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
  //right
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
  //top
    glVertex3f( 0.5f, 1.0f, 0.25f);
    glVertex3f( 0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f,-0.25f);
    glVertex3f(-0.5f, 1.0f, 0.25f);
  //bottom
    glVertex3f( 0.5f,-1.0f, 0.25f);
    glVertex3f( 0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f,-0.25f);
    glVertex3f(-0.5f,-1.0f, 0.25f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  }
void DrawShoulder(){

    glColor3f(0.5,0.5,0.5);
  glBegin(GL_QUADS);
  //forward
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
  //left
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f, 0.25f);
  //back
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
  //right
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
  //top
    glVertex3f( 1.0f, 0.2f, 0.25f);
    glVertex3f( 1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f,-0.25f);
    glVertex3f(-1.0f, 0.2f, 0.25f);
  //bottom
    glVertex3f( 1.0f,-0.2f, 0.25f);
    glVertex3f( 1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f,-0.25f);
    glVertex3f(-1.0f,-0.2f, 0.25f);
  glEnd();
}
void DrawWaise(){
  glColor3f(1.0f,1.0f,1.0f);
  glBegin(GL_QUADS);
    //forward
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
  //left
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f, 0.25f);
  //back
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
  //right
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
  //top
    glVertex3f( 0.7f, 0.2f, 0.25f);
    glVertex3f( 0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f,-0.25f);
    glVertex3f(-0.7f, 0.2f, 0.25f);
  //bottom
    glVertex3f( 0.7f,-0.2f, 0.25f);
    glVertex3f( 0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f,-0.25f);
    glVertex3f(-0.7f,-0.2f, 0.25f);
  glEnd();
}
void DrawArmA(){

    glColor4f(0.82,0.4,0.0,1);
    glBegin(GL_QUADS);

  //forward
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
  //left
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
  //back
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
  //right
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
  //top
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
  //bottom
    glVertex3f( 0.2f,-0.2f, 0.25f);
    glVertex3f( 0.2f,-0.2f,-0.25f);
    glVertex3f(-0.2f,-0.2f,-0.25f);
    glVertex3f(-0.2f,-0.2f, 0.25f);


  glEnd();
}
void DrawArmB(){

    glColor4f(0.0,0.0,0.0,1);
  glBegin(GL_QUADS);

  //forward
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
  //left
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
  //back
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
  //right
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f, 0.25f);
  //top
    glVertex3f( 0.2f, 0.2f, 0.25f);
    glVertex3f( 0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f,-0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
  //bottom
    glVertex3f(-0.2f,-1.0f, 0.25f);
    glVertex3f( 0.2f,-1.0f, 0.25f);
    glVertex3f( 0.2f,-1.0f,-0.25f);
    glVertex3f(-0.2f,-1.0f,-0.25f);
  glEnd();
}
void DrawLeg() {

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pants);
	glBegin(GL_QUADS);
	/*glVertex3f(-0.3f, 0.25f, 0.0f);
	glVertex3f( 0.3f, 0.25f, 0.0f);
	glVertex3f( 0.3f,-3.0f, 0.0f);
	glVertex3f(-0.3f,-3.0f, 0.0f);*/
	//forward
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, 0.25f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3f, 0.25f, 0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, -1.25f, 0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, -1.25f, 0.255f);

	//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3f, 0.25f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3f, 0.25f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, -1.25f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -1.25f, 0.255f);

	//back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3f, 0.25f, -0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 0.25f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3f, -1.25f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -1.25f, -0.255f);

	//right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, 0.25f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 0.25f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3f, -1.25f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, -1.25f, 0.255f);

	//top
	glVertex3f(0.3f, 0.25f, 0.255f);
	glVertex3f(0.3f, 0.25f, -0.255f);
	glVertex3f(-0.3f, 0.25f, -0.255f);
	glVertex3f(-0.3f, 0.25f, 0.255f);


	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void DrawLeg2() {

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pants);
	glBegin(GL_QUADS);
	/*glVertex3f(-0.3f, 0.25f, 0.0f);
	glVertex3f( 0.3f, 0.25f, 0.0f);
	glVertex3f( 0.3f,-3.0f, 0.0f);
	glVertex3f(-0.3f,-3.0f, 0.0f);*/
	//forward
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, -1.5f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3f, -1.5f, 0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, -3.0f, 0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, -3.0f, 0.255f);
	//left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3f, -1.5f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3f, -1.5f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, -3.0f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -3.0f, 0.255f);
	//back
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3f, -1.5f, -0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, -1.5f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3f, -3.0f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -3.0f, -0.255f);
	//right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, -1.5f, 0.255f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, -1.5f, -0.255f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3f, -3.0f, -0.255f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, -3.0f, 0.255f);
	//bottom
	glVertex3f(0.3f, -3.0f, 0.255f);
	glVertex3f(0.3f, -3.0f, -0.255f);
	glVertex3f(-0.3f, -3.0f, -0.255f);
	glVertex3f(-0.3f, -3.0f, 0.255f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawHead(){
  glColor4f(1.0f,1.0f,1.0f, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, face);
    glBegin(GL_QUADS);

  //forward
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.6f, 0.6f, 0.35f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.6f,-0.6f, 0.35f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.6f,-0.6f, 0.35f);
  //left
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f, 0.35f);
  //back


  //right
    glVertex3f(-0.6f, 0.6f, 0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f, 0.35f);
  //top
    glVertex3f( 0.6f, 0.6f, 0.35f);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f, 0.35f);
  //bottom
    glVertex3f( 0.6f,-0.6f, 0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f, 0.35f);

    glColor3f(0,0,0);
    glVertex3f( 0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f, 0.6f,-0.35f);
    glVertex3f(-0.6f,-0.6f,-0.35f);
    glVertex3f( 0.6f,-0.6f,-0.35f);
  glEnd();
  glPushMatrix();
  glColor3f(0.38,0.2,0);
  glScaled(1.5,0.6,1);
  glTranslated(0,1,0);
  glutSolidCube(1);
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}
void DrawNick(){
  glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);

  //forward
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
  //left
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f, 0.25f);
  //back
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
  //right
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);
  //top
    glVertex3f( 0.2f, 0.1f, 0.25f);
    glVertex3f( 0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f,-0.25f);
    glVertex3f(-0.2f, 0.1f, 0.25f);
  //bottom
    glVertex3f( 0.2f,-0.1f, 0.25f);
    glVertex3f( 0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f,-0.25f);
    glVertex3f(-0.2f,-0.1f, 0.25f);

  glEnd();
}

void DrawRobot()
{
	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -40.0f);
	glRotated(90, 0, 1, 0);
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();

	glTranslatef(0.0f, 1.2f, 0.0f);
	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(angleoflarma, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(-90, 1, 0, 0);
	DrawArmA();
	glPopMatrix();
	DrawArmB();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.8, 0.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(angleofrarma, 1, 0, 0);
	glPushMatrix();
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(-90, 1, 0, 0);
	DrawArmA();
	glPopMatrix();
	DrawArmB();
	glPopMatrix();


	//return shoulder
	glPushMatrix();
	glTranslatef(0.0, 0.3, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	DrawHead();
	glPopMatrix();
	DrawNick();
	glPopMatrix();
	DrawShoulder();

	glPopMatrix();

	//return body
	glPushMatrix();
	glTranslatef(0.0f, -1.2f, 0.0f);

	glPushMatrix();
	glTranslatef(0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(angleoflleg, 1, 0, 0);
	DrawLeg();

	glPushMatrix();
	glTranslatef(0.0, -1.6, 0.0);
	glRotatef(angleoflleg1, 1, 0, 0);
	glTranslatef(0.0, 1.6, 0.0);
	DrawLeg2();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef(angleofrleg, 1, 0, 0);
	DrawLeg();

	glPushMatrix();
	glTranslatef(0.0, -1.6, 0.0);
	glRotatef(angleofrleg1, 1, 0, 0);
	glTranslatef(0.0, 1.6, 0.0);
	DrawLeg2();
	glPopMatrix();

	glPopMatrix();

	DrawWaise();
	glPopMatrix();


	DrawBody();
	glPopMatrix();
	//glPopMatrix();
	if (!flag)
	{

		angleoflarma += a;
		angleofrarma -= a;
		angleoflleg -= b;
		angleofrleg += b;

		if (angleoflleg > 0 && b < 0)
		{
			angleoflleg1 += fabs(2*b);
		}
		else if(angleoflleg >= 0 && b > 0)
		{
			
			angleoflleg1 -= fabs(2*b);
		}

		if (angleofrleg > 0 && b > 0)
		{
			angleofrleg1 += fabs(2 * b);
		}
		else if(angleofrleg >= 0 && b < 0)
		{
			angleofrleg1 -= fabs(2 * b);
		}

		if (fabs(angleoflleg) < 0.0001)
			angleoflleg1 = 0;
		if (fabs(angleofrleg) < 0.0001)
			angleofrleg1 = 0;

		if (angleoflarma > 50) {
			a = -aa;
			b = -bb;
		}
		if (angleoflarma < -50) {
			a = aa;
			b = bb;
		}
	}
}

void DrawDownRobot()
{
	glPushMatrix();
	glTranslatef(0.0f, -1.5f, -40.0f);
	glRotated(90, 0, 1, 0);
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-90, 0, 1, 0);



	glPushMatrix();
	glRotatef(-20, 1, 0, 0);
	glTranslatef(0.0f, 0.0f, -0.6f);
	glPushMatrix();
	glTranslatef(0.0f, 1.2f, 0.0f);

	glPushMatrix();
	glTranslatef(0.8, 0.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	//glRotatef(angleoflarma,1,0,0);
	glPushMatrix();
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(-90, 1, 0, 0);
	DrawArmA();
	glPopMatrix();
	DrawArmB();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 0.0, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	//glRotatef(angleofrarma,1,0,0);
	glPushMatrix();
	glTranslatef(0.0, -0.8, 0.0);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(-90, 1, 0, 0);
	DrawArmA();
	glPopMatrix();
	DrawArmB();
	glPopMatrix();

	//return shoulder
	glPushMatrix();
	glTranslatef(0.0, 0.3, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	DrawHead();
	glPopMatrix();
	DrawNick();
	glPopMatrix();
	DrawShoulder();
	glPopMatrix();
	//return body

	glPushMatrix();
	glTranslatef(0.0f, -1.2f, 0.0f);


	glPushMatrix();
	glTranslatef(0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glRotated(-40, 1, 0, 0);
	DrawLeg();
	glPushMatrix();
	glRotated(-35, 1, 0, 0);
	glTranslatef(0.0, 2.0, -0.8);
	DrawLeg2();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);
	glRotated(-40, 1, 0, 0);
	DrawLeg();
	glPushMatrix();
	glRotated(-35, 1, 0, 0);
	glTranslatef(0.0, 2.0, -0.8);
	DrawLeg2();
	glPopMatrix();
	glPopMatrix();

	DrawWaise();
	glPopMatrix();

	DrawBody();
	glPopMatrix();


	glPopMatrix();
}
