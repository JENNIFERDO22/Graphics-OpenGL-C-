#include "stdafx.h"

#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES // Required to access M_PI
#include <math.h> // Required to access sin() and cos()
#include <freeglut.h>
#include <cmath>

void draw_pendulum(float); // Prototype for function to draw pendulum
void text(int, int, char *); 

using namespace std;

float position=0.0;
float velocity=0.0; 
float time=0.0;
float length=0.994;
float g=9.81;
//float dt=0.005; // 5ms

float dt=0.020; // 20ms

int number_of_points=0;
const int max_number_of_points=10000;
float position_array[max_number_of_points];
float velocity_array[max_number_of_points];


float max_position=0.001;
float max_velocity=0.001;
float min_position=0.0;
float min_velocity=0.0; 

float mxxscreen=768+200;
float mnxscreen=768-200;
float mxyscreen=256+200;
float mnyscreen=256-200;
float mx=1, cx=0;
float my=1, cy=0; 

float ball_x_position=0;
float ball_y_position=0; 

bool hello=false; 

void draw_pendulum(float); // Prototype for function 

void draw_attractor(void);

static void display()
{
	 glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	 glColor3f(0.0, 0.0, 1.0); // Draw blue rectangle
	 glBegin(GL_LINE_STRIP);
		 glVertex2d(mnxscreen-4, mnyscreen-4);
		 glVertex2d(mxxscreen+4, mnyscreen-4);
		 glVertex2d(mxxscreen+4, mxyscreen+4);
		 glVertex2d(mnxscreen-4, mxyscreen+4);
		 glVertex2d(mnxscreen-4, mnyscreen-4);
	 glEnd(); 


	 glColor3f(0.0, 0.0, 1.0); // Draw a blue rectangle
	 glBegin(GL_LINE_STRIP);
		 glVertex2d(56, 56);
		 glVertex2d(456, 56);
		 glVertex2d(456, 456);
		 glVertex2d(56, 456);
		 glVertex2d(56, 56);
	 glEnd();

	 draw_pendulum(position); 
	 draw_attractor(); 

	if(hello)
	{
		char str[40]; // Text rendering is Slowwwww!!!!
		 text(mnxscreen-80, (mxyscreen+mnyscreen)/2, "Velocity");
		 sprintf(str,"%3.3f",max_velocity);

		 text(mnxscreen-80, mxyscreen, str);
		 sprintf(str,"%3.3f",min_velocity);

		 text(mnxscreen-80, mnyscreen, str);
		 text(((mxxscreen+mnxscreen)/2)-30, mnyscreen-30, "Position");
		 sprintf(str,"%3.3f",max_velocity);

		 text(mxxscreen, mnyscreen-30, str);
		 sprintf(str,"%3.3f",min_velocity);

		 text(mnxscreen, mnyscreen-30, str);
		 text(((mxxscreen+mnxscreen)/2)-30, mxyscreen+20, "Attractor");
		 text(220, mxyscreen+20, "Pendulum");
	} 

	 glutSwapBuffers();
}


void draw_attractor()
{
	 glColor3f(0.0, 1.0, 0.0); // Draw the attractor

	if(number_of_points>2)
	 {
	 glBegin(GL_LINE_STRIP);
			int count=0;
			for(int i=0;i<number_of_points;i++)
			 {
				if((mx*fabs(ball_x_position - position_array[i])+my*fabs(ball_y_position - velocity_array[i]))<30)
				 {
					count=50;
				}	

				 if(count>0)
				 {
					glColor3f(0.0, 1.0, 0.0);
				 }
				 else
				 {
					glColor3f(1.0, 0.0, 1.0);
				 }

				count--;

				 if(count==0)
				 {
					 glEnd();

					 glBegin(GL_POLYGON); // Locate the current p,v

					 for(float angle=0;angle<=(M_PI*2);angle+=M_PI/3)
					 {

						glVertex2d(cx+(position_array[i]*mx)+(10*cos(angle)),cy+(velocity_array[i]*my)+(10*sin(angle)));
					 }

					glEnd();

					 glBegin(GL_LINE_STRIP);
					 count=0;
				 }

				glVertex2d(cx+(position_array[i]*mx),cy+(velocity_array[i]*my));
			 }
		 glEnd();
	 }

	 glColor3f(1.0, 0.0, 0.0);

	 glBegin(GL_POLYGON); // Locate the current p,v
		for(float angle=0;angle<=(M_PI*2);angle+=M_PI/3)
		 {
			glVertex2d(cx+(position_array[number_of_points - 1]*mx)+(10*cos(angle)),cy+(velocity_array[number_of_points-1]*my)+(10*sin(angle)));
		 }
	 glEnd(); 

	 glColor3f(0.0, 0.0, 1.0); // Draw the circle at mouse click point
	 glBegin(GL_LINE_STRIP);
		for(float angle=0;angle<=(M_PI*2)+0.001;angle+=M_PI/8)
		 {
			glVertex2d(cx+(ball_x_position*mx)+(20*cos(angle)),cy+(ball_y_position*my)+(20*sin(angle)));
		 }
	 glEnd();
} 


/*
void draw_attractor()
{
	 glColor3f(0.0, 1.0, 0.0); // Draw the attractor

	if(number_of_points>2)
	 {
	 glBegin(GL_LINE_STRIP);
	 int count=0;
	 for(int i=0;i<number_of_points;i++)
	 {
	 glVertex2d(cx+(position_array[i]*mx),cy+(velocity_array[i]*my));
	 }
	 glEnd();
	 }
} 
*/


void draw_pendulum(float theta)
{
	float t=theta-(M_PI/2.0); // Subtract 90 degs (down is theta=0)
	 glLineWidth(2.0f);

	float bob_x=256+(150.0*cos(t));
	float bob_y=256+(150.0*sin(t));

	 glColor3f(1.0, 0.0, 0.0); // Draw the string
	 glBegin(GL_LINES);
		 glVertex2d(256, 256);
		 glVertex2d(bob_x,bob_y);
	 glEnd();

	 glBegin(GL_POLYGON); // Draw the bob
		for(float angle=0;angle<=(M_PI*2);angle+=M_PI/3)
		 {
			glVertex2f(bob_x+(10*cos(angle+t)),bob_y+(10*sin(angle+t)));
		 }

	 glLineWidth(1.0f);
	 glEnd();
}

static void idle(int dummy) 
{
	 position += velocity*dt;
	 velocity-=( 2.0 * ((g/length)*sin(position)) +(velocity*0.0f)+(0.5f*(sin(1.1*sqrt(g/length)*time))) )*dt; // valentine’s day

	//velocity -= (( 0.5*(g/length)*sin(position) + 2.0*exp(-time)*cos(2*M_PI*time)) )*dt;
	 //velocity -= ((g/length)*sin(position) + sin (2*sqrt(g/length)*time))*dt;
	 time=time+dt; 

	 
	 position_array[number_of_points]=position;
	velocity_array[number_of_points]=velocity;
	number_of_points++;
	 if(number_of_points>max_number_of_points) number_of_points--; // or set back to zero? 
	 

	 bool f=false; // Rescale if new point exceeds exisitng bounds
	if(position>max_position){max_position=position; f=true;}
	if(velocity>max_velocity){max_velocity=velocity; f=true;}
	if(position<min_position){min_position=position; f=true;}
	if(velocity<min_velocity){min_velocity=velocity; f=true;} 
	if(f)
	{
	mx=(mxxscreen-mnxscreen)/(max_position-min_position);
	 cx=mnxscreen-(mx*min_position);
	 my=(mxyscreen-mnyscreen)/(max_velocity-min_velocity);
	 cy=mnyscreen-(my*min_velocity);
	} 

	 if (!hello) glutTimerFunc(1000*dt,idle,0);

	 glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	 {
		 if (state == GLUT_DOWN)
		 { // Convert screen position to position in attractor space
			 ball_x_position=(x-cx)/mx;
			 ball_y_position=((glutGet(GLUT_WINDOW_HEIGHT)-y)-cy)/my;
		 }
	 }

	if (button == GLUT_RIGHT_BUTTON)
	 {
		 if (state == GLUT_DOWN)
		 {
			 if(hello)
			 {
				 hello=false;
				 glutTimerFunc(1000*dt,idle,0);
			 }
			 else
			 {
				hello=true;
			 }
		 }
	 } 

} 




// Function to add text to a graphics display
void text(int x, int y, char *string)
{
	 glDisable(GL_TEXTURE_2D);
	 glColor4f( 1.0f, 0.0f, 0.0f, 1.0f ); // Red

	 glRasterPos2f(x, y);

	 int len, i;
	 len = (int)strlen(string);

	 for (i = 0; i < len; i++)
	 {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]); // Other fonts possible
	 }
} 




int _tmain(int argc, char** argv)
{
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	 glutInitWindowSize(1024,512);
	 glutInitWindowPosition(10,10);

	 glutCreateWindow("Pendulum");
	 glLoadIdentity();

	 glMatrixMode(GL_PROJECTION);
	 gluOrtho2D(0.0, 1024.0, 0.0, 512.0);

	 glutDisplayFunc(display);
	  glutTimerFunc(1000.0*dt, idle, 0); 
	 //glutIdleFunc(idle);
	 glutMouseFunc(mouse); 
	

	 cout << position_array[0] << endl;

	 glutMainLoop();
	return 0;
}