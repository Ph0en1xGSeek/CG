#include "cars.h"
//make a global variable -- for tracking the anglular position of camera
double cameraAngle;			//in radian
double cameraAngleDelta;

double cameraHeight;
double cameraHeightDelta = 0.05;
double cameraRadius;

double rectAngle;	//in degree
double lamp_start;

double car_position;

bool canDrawGrid;
bool camera_angle_inc;
bool camera_height_inc;

double angle = 120;



GLUquadric *qobj = gluNewQuadric();;

void car_top(double x, double y){
    //car top
   // x = 7.5;
    glBegin(GL_QUADS);{
            glColor3f(LIGHT_GREEN);
            glVertex3f(x-7.5,0,30);
            glVertex3f(x+7.5,0,30);//right low
            glColor3f(DARK_GREEN);
            glVertex3f(x+7.5,30,30);//right top
            glVertex3f(x-7.5,30,30);
        }glEnd();

     // between top and front glass
        glBegin(GL_QUADS);{
            glColor3f(LIGHT_GREEN);
            glVertex3f(x-7.5,30,30);
            glVertex3f(x+7.5,30,30);//right low
            glVertex3f(x+7.5,32,28);//right top
            glVertex3f(x-7.5,32,28);
        }glEnd();

    // between front and back glass
        glBegin(GL_QUADS);{
            glColor3f(LIGHT_GREEN);
            glVertex3f(x-7.5,0,30);
            glVertex3f(x-7.5,-3,27);

            glColor3f(DARK_GREEN);
            glVertex3f(x,-3,27);//rihgt low
            glVertex3f(x,0,30);//right top
        }glEnd();

        glBegin(GL_QUADS);{
            glColor3f(LIGHT_GREEN);
            glVertex3f(x+7.5,-3,27);//rihgt low
            glVertex3f(x+7.5,0,30);//right top

            glColor3f(DARK_GREEN);
            glVertex3f(x,0,30);//right top
            glVertex3f(x,-3,27);//rihgt low

        }glEnd();
}

void front_n_back_glass(double x, double y){
    // front glass

        glBegin(GL_QUADS);{
            glColor3f(DARK_WHITE);
            glVertex3f(x - 7.5,32,28);
            glVertex3f(x+7.5,32,28);//right low
            glColor3f(LIGHT_WHITE);
            glVertex3f(x+11.5,40,20);//right top
            glVertex3f(x-11.5,40,20);
        }glEnd();

        // back glass
        glBegin(GL_QUADS);{
            glColor3f(DARK_WHITE);
            glVertex3f(x-7.5,-3,27);
            glVertex3f(x+7.5,-3,27);//right top
            glColor3f(LIGHT_WHITE);
            glVertex3f(x+10.5,-9,20);//right low
            glVertex3f(x-10.5,-9,20);
        }glEnd();
}

void top_left_frame(double x, double y){
    glBegin(GL_POLYGON);{
            /* Set the vertices */
            glColor3f(LIGHT_GREEN);
            //front glass
            glVertex3f(x-11.5,40,20);//front right top
            glVertex3f(x-7.5,32,28);//front right low
            glVertex3f(x-7.5,30,30);//between right low
            //middhel of front glass nad top base
            glColor3f(DARK_GREEN);
            glVertex3f(x-8,30,28);//right low
            glVertex3f(x-13.5,40,20);
        }glEnd();

        //base line
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                //front glass

                glVertex3f(x-7.5,30,30);//between right low
                glVertex3f(x-7.5,0,30);//base right low

                glColor3f(DARK_GREEN);
                //middhel of front glass nad top base
                glVertex3f(x-7,0,28);//base right low
                glVertex3f(x-7,30,28);//right low
            }glEnd();

         // back glass side
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-7.5,0,30);//base right low
                glVertex3f(x-7.5,-3,27);//rihgt low
                glVertex3f(x-10.5,-9,20);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5,-9,20);//right low
                glVertex3f(x-7,0,28);//base right low
                glVertex3f(x-7.5,0,30);//base right low

            }glEnd();


            //horizontal border back glass to front glass
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-13.5,-9,20);//right low
                glVertex3f(x-14.5,-9,18);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,40,18);
                glVertex3f(x-13.5,40,20);//
                //front right top

            }glEnd();
}

void top_right_frame(double x,double y){
    glBegin(GL_POLYGON);{
            /* Set the vertices */
            glColor3f(LIGHT_GREEN);
            //front glass
            glVertex3f(x+11.5,40,20);//front right top
            glVertex3f(x+7.5,32,28);//front right low
            glVertex3f(x+7.5,30,30);//between right low
            //middhel of front glass nad top base
            glColor3f(DARK_GREEN);
            glVertex3f(x+8,30,28);//right low
            glVertex3f(x+13.5,40,20);
        }glEnd();

        //base line
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                //front glass

                glVertex3f(x+7.5,30,30);//between right low
                glVertex3f(x+7.5,0,30);//base right low

                glColor3f(DARK_GREEN);
                //middhel of front glass nad top base
                glVertex3f(x+8,0,28);//base right low
                glVertex3f(x+8,30,28);//right low
            }glEnd();

         // back glass side
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+7.5,0,30);//base right low
                glVertex3f(x+7.5,-3,27);//rihgt low
                glVertex3f(x+10.5,-9,20);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,-9,20);//right low
                glVertex3f(x+8,0,28);//base right low
                glVertex3f(x+7.5,0,30);//base right low

            }glEnd();


            //horizontal border back glass to front glass
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+13.5,-9,20);//right low
                glVertex3f(x+14.5,-9,18);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x+14.5,40,18);
                glVertex3f(x+13.5,40,20);//
                //front right top

            }glEnd();
}


void right_side_window(double x, double y){
    //right side front galass window/////////////////////////////////////////////////////////////////////////////////
            glBegin(GL_POLYGON);{
                glColor3f(DARK_WHITE);
                glVertex3f(x+13.5,40,20);//

                glColor3f(LIGHT_WHITE);
                glVertex3f(x+8,30,28);//right low
                glVertex3f(x+13.5,30,20);//
                //front right top
            }glEnd();
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_WHITE);
                glVertex3f(x+8,30,28);
                glVertex3f(x+13.5,30,20);//

                glColor3f(DARK_WHITE);
                glVertex3f(x+13.5,18,20);//
                glVertex3f(x+8,17.5,28);//right low
            }glEnd();

            //right side middle window
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(DARK_WHITE);

                glVertex3f(x+8, 16.5  , 28);//right low
                glVertex3f(x+13.5  , 16.5  , 20);//

                glColor3f(LIGHT_WHITE);
                glVertex3f(x+13.5,1,20);//
                glVertex3f(x+8,1.5,28);//right low
            }glEnd();

            //right side back window
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_WHITE);
                glVertex3f(x+13.5,0,20);//
                glVertex3f(x+8,0.9,28);
                glVertex3f(x+8,0,28);

                glColor3f(DARK_WHITE);
                glVertex3f(x+13.5,-9,20);//right low
            }glEnd();
}


void left_side_window(double x, double y){
    //right side front galass window/////////////////////////////////////////////////////////////////////////////////
            glBegin(GL_POLYGON);{
                glColor3f(DARK_WHITE);
                glVertex3f(x-13.5,40,20);//

                glColor3f(LIGHT_WHITE);
                glVertex3f(x-8,30,28);//right low
                glVertex3f(x-13.5,30,20);//
                //front right top
            }glEnd();
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_WHITE);
                glVertex3f(x-8,30,28);
                glVertex3f(x-13.5,30,20);//

                glColor3f(DARK_WHITE);
                glVertex3f(x-13.5,18,20);//
                glVertex3f(x-8,17.5,28);//right low
            }glEnd();

            //right side middle window
            glBegin(GL_POLYGON);{
                /* Set the vertices */
                glColor3f(DARK_WHITE);

                glVertex3f(x-8, 16.5  , 28);//right low
                glVertex3f(x-13.5  , 16.5  , 20);//

                glColor3f(LIGHT_WHITE);
                glVertex3f(x-13.5,1,20);//
                glVertex3f(x-8,1.5,28);//right low
            }glEnd();

            //right side back window
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_WHITE);
                glVertex3f(x-13.5,0,20);//
                glVertex3f(x-8,0.9,28);
                glVertex3f(x-8,0,28);

                glColor3f(DARK_WHITE);
                glVertex3f(x-13.5,-9,20);//right low
            }glEnd();
}


void right_side_window_divider(double x, double y){
             //glass devider one
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+8,17.5,28);
                glVertex3f(x+8, 16.5  , 28);

                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5 , 16.5  , 20);
                glVertex3f(x+13.5,18,20);
                //right low
            }glEnd();

            //glass devider two
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+8,0.9,28);
                glVertex3f(x+8,1.5,28);

                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,1,20);
                 glVertex3f(x+13.5,0,20);
            }glEnd();
}

void left_side_window_divider(double x, double y){
             //glass devider one
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-8,17.5,28);
                glVertex3f(x-8, 16.5  , 28);

                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5  , 16.5  , 20);
                glVertex3f(x-13.5,18,20);
                //right low
            }glEnd();

            //glass devider two
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-8,0.9,28);
                glVertex3f(x-8,1.5,28);

                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5,1,20);
                 glVertex3f(x-13.5,0,20);
            }glEnd();
}

void back_side_body(double x,double y){
    //back side of back glass
            glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,-9,20);//right low
                glVertex3f(x-13.5,-9,20);//right low

                glColor3f(LIGHT_GREEN);
                glVertex3f(x-14.5,-9,18);//right low
                glVertex3f(x+14.5,-9,18);//right low
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-14.5,-9,18);//right low
                glVertex3f(x+14.5,-9,18);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x+14.5,-10,15);//right low
                glVertex3f(x-14.5,-10,15);//right low
            }glEnd();

            // red light left
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_RED);
                glVertex3f(x-9,-10,15);//right low
                glVertex3f(x-14.5,-10,15);//right low

                glColor3f(DARK_RED);
                glVertex3f(x-14.5,-10.2,11.2);//right low
                glVertex3f(x-9,-10.2,11.2);//right low
            }glEnd();

            // red light right
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_RED);
                glVertex3f(x+9,-10,15);//right low
                glVertex3f(x+14.5,-10,15);//right low

                glColor3f(DARK_RED);
                glVertex3f(x+14.5,-10.2,11.2);//right low
                glVertex3f(x+9,-10.2,11.2);//right low
            }glEnd();



            //number board
            glBegin(GL_QUADS);{
                glColor3f(x-6.998, 0.502, 0.502);
                glVertex3f(x-4, -10.04, 15);//right low
                glVertex3f(x+4, -10.04, 15);//right low

                glColor3f(x-6.502, 0.910, 0.667);
                glVertex3f(x+4, -10.24, 11.2);//right low
                glVertex3f(x-4, -10.24, 11.2);//right low

            }glEnd();
            // place between two lines
            glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x+9,-10,15);//right low
                glVertex3f(x-9,-10,15);//right low

                glColor3f(LIGHT_GREEN);
                glVertex3f(x-9,-10.2,11.2);//right low
                glVertex3f(x+9,-10.2,11.2);//right low

            }glEnd();

            //last part of back side
            glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+14.5,-10.2,11.2);//right low
                glVertex3f(x-14.5,-10.2,11.2);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,-10.5,7);//right low
                glVertex3f(x+15,-10.5,7);//right low

            }glEnd();
}

void create_triangle(double x, double y,double z,double radius, double resolution)
{
    /* top triangle */
    double PI = 3.1416;
    double i;
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, z);  /* center */
            for (i = 0; i <= 2 * PI; i += resolution)
                glVertex3f(x, y + radius*cos(i), z+radius * sin(i));
        glVertex3f(x, y + radius*cos(i), z + radius*sin(i));
    glEnd();
}

void create_wheel(double x, double y, double z, double radious, double width){
    glPushMatrix();
    {
        glColor3f(0,0,0);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, z);
        glRotatef(90,0,1,0);

        gluCylinder(qobj, radious, radious, width, 15, 20);
    }
    glPopMatrix();

    glColor3f(0.09,0.09,0.09);
    create_triangle(x+width,y, z, radious, 0.3);
    create_triangle(x,y, z, radious, 0.3);
}

void front_body(double x, double y)
{
    //between front and front glass
        glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+13.5,40,20);//right low
                glVertex3f(x-13.5,40,20);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,40,18);//right low
                glVertex3f(x+14.5,40,18);//right low
            }glEnd();
        //front hood
        glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,40,18);//right low
                glVertex3f(x+14.5,40,18);//right low

                glColor3f(LIGHT_GREEN);
                glVertex3f(x+13.5,52,15);//right low
                glVertex3f(x-13.5,52,15);//right low
            }glEnd();
        //small curv after front hood
        glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-13.5,52,15);//right low
                glVertex3f(x+13.5,52,15);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,53,12);//right low
                glVertex3f(x-13.5,53,12);//right low
            }glEnd();
        //front end
        glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5,53,12);//right low
                glVertex3f(x+13.5,53,12);//right low

                glColor3f(LIGHT_GREEN);
                glVertex3f(x+13.5,54,7);//right low
                glVertex3f(x-13.5,54,7);//right low
            }glEnd();

       //front right
       glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,54,7);//right low
                glVertex3f(x+13.5,53,12);//right low
                glVertex3f(x+13.5,52,15);//right low
                glVertex3f(x+14.5,40,18);//right low
                glVertex3f(x+13.5,40,7);//right low
       }glEnd();

       //front left
       glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5,54,7);//right low
                glVertex3f(x-13.5,53,12);//right low
                glVertex3f(x-13.5,52,15);//right low
                glVertex3f(x-14.5,40,18);//right low
                glVertex3f(x-13.5,40,7);//right low
       }glEnd();

}

void car_doors(double x, double y){
     //doors right
       glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x+14.5,40,18);//right low
                glColor3f(LIGHT_GREEN);
                 glVertex3f(x+14.5,10,18);//right low


                glColor3f(DARK_GREEN);
                glVertex3f(x+13.5,10,7);//right low
                glVertex3f(x+13.5,40,7);//right low

       }glEnd();
        glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x+14.5,10,18);//right low
                glVertex3f(x+14.5,-9,18);//right low
                glVertex3f(x+14.5,-10,15);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x+14.5,-10.2,11.2);//right low
                glVertex3f(x+15, -10.5, 7);//right low
                glVertex3f(x+13.5,10,7);//right low

       }glEnd();

        //doors left
       glBegin(GL_POLYGON);{
                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,40,18);//right low
                glColor3f(LIGHT_GREEN);
                 glVertex3f(x-14.5,10,18);//right low


                glColor3f(DARK_GREEN);
                glVertex3f(x-13.5,10,7);//right low
                glVertex3f(x-13.5,40,7);//right low

       }glEnd();
        glBegin(GL_POLYGON);{
                glColor3f(LIGHT_GREEN);
                glVertex3f(x-14.5,10,18);//right low
                glVertex3f(x-14.5,-9,18);//right low
                glVertex3f(x-14.5,-10,15);//right low

                glColor3f(DARK_GREEN);
                glVertex3f(x-14.5,-10.2,11.2);//right low
                glVertex3f(x-14.5, -10.5, 7);//right low
                glVertex3f(x-13.5,10,7);//right low

       }glEnd();
}

void draw_car(double x, double y){

    glPushMatrix();
    glRotated(angle, 0,1,0);
    glTranslated(130, -40, 0);
    glRotated(-90, 1,0,0);
    car_top(x,y);
    front_n_back_glass(x,y);

    top_right_frame(x,y);
    right_side_window(x,y);
    right_side_window_divider(x,y);

    top_left_frame(x,y);
    left_side_window(x,y);
    left_side_window_divider(x,y);

    back_side_body(x,y);
    front_body(x,y);

    car_doors(x,y);
    create_wheel(x+11, 0, 7, 6, 4);
    create_wheel(x-15, 0, 7, 6, 4);

    create_wheel(x+11, 40, 7, 6, 4);
    create_wheel(x-15, 40, 7, 6, 4);
    glPopMatrix();
    angle += 0.1;
}

void create_lamp(double x,double y,double z,double height){
    double radius = 2.00*height/100.00;

    glPushMatrix();
    {
        glColor3f(LAMP_COLOR);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, z);
        glRotatef(90,0,0,1);
        // qobject, radious bottom, radious top, height, slice , slice
        gluCylinder(qobj, radius, 3.00*radius/2.00, 5.00*height/100.00, 10, 10);
    }glPopMatrix();
    glPushMatrix();
    {
        glColor3f(LAMP_COLOR);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, z+5.00*height/100.00);
        glRotatef(90,0,0.0,1);
        // qobject, radious bottom, radious top, height, slice , slice
        gluCylinder(qobj, 3.00*radius/2.00, 3.00*radius/2.00, 0.5*height/100.00, 10, 10);
    }glPopMatrix();

    glPushMatrix();
    {
        glColor3f(LAMP_COLOR);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, z+5.5*height/100.00);
        glRotatef(90,0,0.0,1);
        // qobject, radious bottom, radious top, height, slice , slice
        gluCylinder(qobj, 3.00*radius/2.00, radius, 5.0*height/100.00, 10, 10);
    }glPopMatrix();

    glPushMatrix();
    {
        glColor3f(LAMP_COLOR);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, z+10.5*height/100.00);
        glRotatef(90,0,0.0,1);
        // qobject, radious bottom, radious top, height, slice , slice
        gluCylinder(qobj, radius, radius/2, 90*height/100.00, 50, 50);
    }glPopMatrix();

    glPushMatrix();
    {
        glColor3f(LAMP_COLOR);
        //glTranslatef(10,-20,50);
        glTranslatef(x, y, height);
        glRotatef(90,0,1,0);
        // qobject, radious bottom, radious top, height, slice , slice
        gluCylinder(qobj, 0.5*radius/2.00, 0.5*radius/2.00, 10*height/100.00, 50, 15);
    }glPopMatrix();

    //void glutSolidSphere(GLdouble radius,GLint slices, GLint stacks);
    double equ[4];

        equ[0] = 1;	//0.x
        equ[1] = 0;	//0.y
        equ[2] = 0;//-1.z x + 10*height/100.00

        equ[3] = (-1.00)*(x + 10*height/100.00);//30

        glClipPlane(GL_CLIP_PLANE0,equ);
        //now we enable the clip plane

   // glEnable(GL_CLIP_PLANE0);{
    glEnable(GL_CLIP_PLANE0);{
        glColor3f(LIGHT_COLOR);
            glPushMatrix();{
                glTranslatef(x+15.00*height/100.00, y+.4, height);
                glScalef(3, 1, 0.5);

                glutSolidSphere(5*height/100.00, 20.00*height/100.00, 20.00*height/100.00);
            }glPopMatrix();
   }glDisable(GL_CLIP_PLANE0);
   //}glDisable(GL_CLIP_PLANE0);
}
