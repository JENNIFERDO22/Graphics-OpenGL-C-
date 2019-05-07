// ThiThuTrang Do Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>    // Required for sound only (consider OpenAL), 
#include <mmsystem.h>   // Again only required for sound only

#include <freeglut.h>   // GLUT OpenGL Utility Toolkit
#include "Sprite.h"     // Prototype Sprite class

// no_balls Sprites are used in the "game"
Sprite player;
const int no_balls = 2;
Sprite balls[no_balls];
Sprite background;
int ball_moving; // if ball_moving == 1, then display the ball and let it falls down
//Sprite catapult;

// Score data and method
int score=0;
int best_score=0;
char score_text[20];
char message[40];
char best_score_text[20];


// Callback function called each time screen is to be redrawn (called by OS not you directly)
static void display(void)                                 
{															// Keep game logic out of display()...
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer

		background.drawSprite();							// Draw background at X,Y in a given direction
		//catapult.drawSprite();                              // Draw catapult
		player.drawSprite();								// Draw player	
		
		for (int i=0; i < no_balls; i++) {
			balls[i].drawSprite();								    // Draw balls
		}

		sprintf(score_text,"Score: %d",score);              // Display text 
		Sprite::text(500,400,score_text);

		sprintf(best_score_text,"Best score: %d",best_score);              // Display text 
		Sprite::text(500,370,best_score_text);

		sprintf(message,"AVOID THE BALLS!!!");              // Display text 
		Sprite::text(500,340,message);

		glutSwapBuffers();  // Display buffer just drawn and then switch to draw on the other buffer
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
	Sprite::spriteModeOn(0,0);  // (0,0) Causes orginal screen size to be reused
}

// Change position and frame of player

// Callback to manage standard keys on the keyboard
// In this case 'q' or 'Q' causes program to terminate
static void qwerty_keys(unsigned char key, int x, int y)
{
    switch (key)
    {
		case 'a':
		case 'A':
			player.advance_frame(1,4);
			player.sprite_x_position-=15;
			if (player.sprite_x_position <= 0) player.sprite_x_position = 10;
			player.direction=true;
			break;


		case 'd': 
		case 'D':
			player.advance_frame(1,4);
			player.sprite_x_position+=15;
			if (player.sprite_x_position > 900) player.sprite_x_position = 900;
			player.direction=false;
			break;

		case 'Q':
		case 'q': 
			glutLeaveMainLoop () ; break;  // (Stop!)

		default: break;
    }
}

// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
static void function_keys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
			player.advance_frame(1,4);
			player.sprite_x_position-=15;
			if (player.sprite_x_position <= 0) player.sprite_x_position = 10;
			player.direction=true;
		break;


    case GLUT_KEY_RIGHT: 
		    player.advance_frame(1,4);
			player.sprite_x_position+=15;
			if (player.sprite_x_position >= 900) player.sprite_x_position = 900;
			player.direction=false;
		break;

    case GLUT_KEY_UP: 
		player.advance_frame(4,7);
		break;

    default: 
		break;
    }
}

// Game loop
// static void idle()    // if using  glutIdleFunc(idle); this line should replace the one below

// change the position and velocity of the ball every frame
static void idle(int v)  // Runs between frame updates in doublebuffer mode, game logic goes here
{	
	for (int i = 0; i <no_balls; i++) {
		ball_moving = rand() % 2;
		if(ball_moving == 1)
		{
		  balls[i].advance_projectile();      // Advance pos and velocity of the ball
		  if(balls[i].sprite_y_position<20)   // if it has landed
		  { 	   
			   score++;
			   if (score > best_score) best_score = score;
			   // set new pos and velocity when the ball is landed
			   balls[i].sprite_y_position=600;
			   balls[i].sprite_y_velocity=1 + (rand()%3);
			   balls[i].sprite_x_position=10 + 10 * (rand()%100);
			   balls[i].sprite_x_velocity=0;
		  }

		  balls[i].advance_frame(0,5);  // Cycle through 6 images [0,5]
		  if(player.collision(balls[i])) score=0; // If player collides with ball score is reset to zero
		} 
	}
	
	
	//if (!ball_moving) 

	//if (!ball_moving) catapult.advance_frame(0,3);
	//if (catapult.frame_no==3)
	//{
    //  ball_moving=true;
	//  catapult.frame_no=0;
	//}

    glutPostRedisplay();      // Send message to ask for screen to be redrawn
	glutTimerFunc(10,idle,0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
}

/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	Sprite::spriteModeOn(1024,600);		// Create a screen 1024x600 to contain game

	// Start playing background music (uses Windows calls, there are other ways incluing OpenAL)
	PlaySound(L"Music.wav",NULL, SND_LOOP| SND_ASYNC);  

	// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
	// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128
	player.load("Spritesheet.bmp",10,1,320,10,150,150,false);

	for (int i=0; i < no_balls; i++) {
		//balls[i].load("Ball6x1.bmp",3,2,100,400,50,50,true);	
		balls[i].load("Ball6x1.bmp",3,2,100,600,50,50,true);	// Draw balls
	}

	background.load("Background.bmp",1,1,0,0,1024,600,true); 
	//catapult.load("Catapult.bmp",4,1,0,0,210,170,true); 

    glutReshapeFunc(resize);			// Called when window is resized (not required)
	glutDisplayFunc(display);			// Callback function to redraw, an event created by glutPostRedisplay()
    glutKeyboardFunc(qwerty_keys);		// QWERTY Keys
    glutSpecialFunc(function_keys);		// Cursor keys etc 

	//glutIdleFunc(idle);			    // Called during idle time 
	glutTimerFunc(10,idle,0);           // Use timer callback for more consistent timing
	
	glutMainLoop();						// Start main loop running (go!)
    
	return EXIT_SUCCESS;
}
