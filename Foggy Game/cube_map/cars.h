
#include<GL/freeglut.h>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>


#define BLACK 0, 0, 0

#define WHITE 1, 1, 1

#define LIGHT_GREEN 0.667, 0.098, 0.125
#define DARK_GREEN 0.400, 0.002, 0.002

#define LIGHT_RED 0.647, 0.165, 0.165
#define DARK_RED 0.502, 0.200, 0.000

#define LIGHT_WHITE 0.961, 0.961, 0.961
#define DARK_WHITE 0.502, 0.502, 0.502
#define DEEP_DARK_WHITE 0.412, 0.412, 0.412


#define START_RILL 45
#define RILL_DELTA 2
#define DEVIDER_LEN 25

#define START_LAMP -200
#define LAMP_DELTA 2
#define END_LAMP -400
#define LAMP_INTERVAL 200

#define START_ANGLE 4
#define END_ANGLE 5.5

#define MAX_CAMERA_HEIGHT 100
#define MIN_CAMERA_HEIGHT 10


#define CAR_POSITION_MIN -18
#define CAR_POSITION_DELTA 0.2
#define CAR_POSITION_MAX 38



#define LAMP_COLOR 0.184, 0.310, 0.310
#define LIGHT_COLOR 0.914, 0.588, 0.478

#define DELTA 0.001


#define SKY_TOP 1.000, 1.000, 1.000


void car_top(double x, double y);
void front_n_back_glass(double x, double y);
void top_left_frame(double x, double y);
void top_right_frame(double x,double y);
void right_side_window(double x, double y);
void left_side_window(double x, double y);
void right_side_window_divider(double x, double y);
void left_side_window_divider(double x, double y);
void back_side_body(double x,double y);
void create_triangle(double x, double y,double z,double radius, double resolution);
void create_wheel(double x, double y, double z, double radious, double width);
void front_body(double x, double y);
void car_doors(double x, double y);
void draw_car(double x, double y);
void create_lamp(double x,double y,double z,double height);
