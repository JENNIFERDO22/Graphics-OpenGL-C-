// flyby.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <math.h>
// glu - OpenGL Utility Library
#include <windows.h> // You may need to go to C/C++, Preprocessor,
 // and in the Preprocessor Definitions field add WIN32
#include <GL/gl.h> // OpenGL Linker add openGL32.lib, gdi32.lib and User32.lib
#include <GL/glu.h>

// Mathematics GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
// Freeglut - The OpenGL Utility Toolkit
#include <freeglut.h>
// Loading Wavefront files
#include<glm.h>
#include<glm.c>
#include<glmimg.c>
#include<glm_util.c>

// Global variables
GLMmodel *objmodel_sky;
//GLMmodel *objmodel_aircraft;
GLMmodel *objmodel_houses;
GLMmodel *objmodel_aircraft; 
GLMmodel *objmodel_rocket; 

glm::vec4 position_aircraft;
glm::quat direction_aircraft;

glm::vec4 change_in_position;
glm::vec4 camera_up;
glm::vec4 camera_position;
glm::vec4 position_rocket;
glm::quat direction_rocket;
float speed_rocket=1;
bool rocket=false; 


float rotation;
//float speed=0.025f;
float speed=0.005f;

static void display(void)
{
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glDisable(GL_LIGHTING);

	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();

	 glPushMatrix();
	glTranslatef(0,-0.25,-2);
	glRotatef(180,0,1,0);
	glmDraw(objmodel_aircraft, GLM_SMOOTH | GLM_COLOR); // aircraft
	glColor3f(1.0, 1.0,1.0);
	glPopMatrix(); 

	if(rocket)
	{
		glPushMatrix();
		glTranslatef(position_rocket.x,position_rocket.y,position_rocket.z); // Translate
		glMultMatrixf( glm::value_ptr( glm::transpose(glm::mat4_cast(direction_rocket))) ); // Rotate
		glTranslatef(0,-0.25,2); // Offset
		glScalef(0.5,0.5,0.5); // Scale
		glmDraw(objmodel_rocket, GLM_SMOOTH | GLM_COLOR); // objmodel_aircraft may look better
		glColor3f(1.0, 1.0,1.0);
		glPopMatrix();
	} 



	// eye, (where the camera is located)
	// target, (where the camera is looking)
	// up, vector of the camera (is Up)
	
	 gluLookAt(camera_position.x, camera_position.y ,camera_position.z,
	 position_aircraft.x, position_aircraft.y, position_aircraft.z,
	 camera_up.x, camera_up.y, camera_up.z);
	 

	 //gluLookAt(0,0,10, 0,0,0, 0,1,0);

	 glPushMatrix();
	 glScalef(300, 300, 300);
	 glmDraw(objmodel_sky, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL ); // skybox
	 glPopMatrix();

	 glPushMatrix();
	 glColor3f(1.0, 1.0, 0.0); // Draw a green quad to divide skybox and extend the terrain
	 glBegin(GL_QUADS);
		 glVertex3f(-300,-1,-300);
		 glVertex3f(-300,-1, 300);
		 glVertex3f(300, -1, 300);
		 glVertex3f(300, -1,-300);
	 glEnd();

	 glColor3f(1.0, 1.0,1.0);
	 glScalef(10, 10, 10);
	 glmDraw(objmodel_houses, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL ); // terrain
	 glPopMatrix();

	 
	 glPushMatrix();
	glTranslatef(0,0.8,-5);
	glRotatef(180 + rotation, 0, 1, 0);
	glTranslatef(0,0,-3);
	glmDraw(objmodel_aircraft, GLM_SMOOTH | GLM_COLOR); // aircraft
	glColor3f(1.0, 1.0,1.0);
	glPopMatrix();

	 glutSwapBuffers();

}

static void keys(int key, int x, int y)
	{ 
	 float yaw=0,pitch=0,roll=0;
	 switch (key)
	 {
		 case GLUT_KEY_LEFT: yaw-=0.02; break;
		 case GLUT_KEY_RIGHT: yaw+=0.02; break;
		 case GLUT_KEY_UP: pitch-=0.02; break;
		 case GLUT_KEY_DOWN: pitch+=0.02; break;
		 case GLUT_KEY_PAGE_UP: roll-=0.02; break;
		 case GLUT_KEY_PAGE_DOWN: roll+=0.02; break;
		 case GLUT_KEY_F1: speed+=0.005f; break;
		 case GLUT_KEY_F2: speed-=0.005f; break;
		case GLUT_KEY_F3: rocket=true; break; 
		 case GLUT_KEY_F12: glutLeaveMainLoop () ; break; // (Stop!)
		 default: break;
	 }
	 // Quaternions are associative (a*b)*c=a*(b*c) but not commutative (a*b)!=(b*a)
	 // The order matters (XNA/Monogame seems to multiply on the other side so *= would work)
		 direction_aircraft = glm::angleAxis(pitch,glm::vec3(1.0f, 0.0f, 0.0f)) * direction_aircraft;
		 direction_aircraft = glm::angleAxis(yaw,glm::vec3(0.0f, 1.0f, 0.0f)) * direction_aircraft;
		 direction_aircraft = glm::angleAxis(roll,glm::vec3(0.0f, 0.0f, -1.0f)) * direction_aircraft;

		 glutPostRedisplay(); // Send message to redraw screen
	}

static void idle()
	{
		 change_in_position = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*glm::toMat4(direction_aircraft);
		 position_aircraft += change_in_position*speed;
		 camera_up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)*glm::toMat4(direction_aircraft);
		 camera_position=position_aircraft-change_in_position; // previous position

		 if(rocket==false)
		{
			position_rocket=position_aircraft;
			direction_rocket=direction_aircraft;
			speed_rocket=speed*1.2;
		}
		else
		{
			position_rocket += glm::vec4(0.0f, 0.0f, speed_rocket, 1.0f)*glm::toMat4(direction_rocket);
		} 

		 rotation += 0.5;
		 if (rotation >360) rotation = 0;

		glutPostRedisplay();
	}

int _tmain(int argc, _TCHAR* argv[])
{
	 glutInit(&argc, (char**)argv);
	 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	 glutInitWindowSize(640,480); // or 800,600
	 glutInitWindowPosition(10,10);

	 glutCreateWindow("Flight simulator");

	 // glutFullScreen(); // Optional
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(40.0, 640.0/480.0, 0.5, 500.0); // or 800.0/600.0

	 glEnable(GL_DEPTH_TEST); // Turn on Z-depth buffering
	 glEnable(GL_CULL_FACE); // Turn on backface culling
	 glFrontFace(GL_CCW);

	 // Load skybox and buildings models
	 objmodel_sky = glmReadOBJ("skybox.obj");
	 glmUnitize(objmodel_sky);
	 glmFacetNormals(objmodel_sky);
	 glmVertexNormals(objmodel_sky, 90.0,GL_FALSE);

	 objmodel_aircraft = glmReadOBJ("biplane2.obj");
	glmUnitize(objmodel_aircraft);
	glmFacetNormals(objmodel_aircraft);
	// glmVertexNormals(objmodel_aircraft, 90.0,GL_FALSE); // Do not include this line

	objmodel_rocket = glmReadOBJ("rocket.obj");
	glmUnitize(objmodel_rocket);
	glmFacetNormals(objmodel_rocket);

	 objmodel_houses = glmReadOBJ("houses.obj");
	 glmUnitize(objmodel_houses);
	 glmFacetNormals(objmodel_houses);
	 glmVertexNormals(objmodel_houses, 90.0,GL_FALSE);

	 // Starting position and direction of aircraft
	 position_aircraft=glm::vec4(0, 1.5f, -25.0f , 1);
	 direction_aircraft=glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, -1.0f));

	 rotation = 0;

	 glutSpecialFunc(keys);
	 glutIdleFunc(idle);
	 glutDisplayFunc(display);
	 glutMainLoop(); // Start Glut main loop

	 return 0;
} 

