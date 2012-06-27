/****************************************************************************
*																			*
*	Program:	TicTacToe.cpp												*
*																			*
*	Author:		Robb T. Koether												*
*																			*
*	Date:		Aug 19, 2011												*
*																			*
*	Purpose:	This program will play a game of Tic Tac Toe.				*
*																			*
/***************************************************************************/

//	Include header files

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <ctime>
#include <cmath>

using namespace std;

//	Constants

const float PI = 3.14159265358979323;
const int ESC = 27;

float BOARD_WIDTH = 120;  // Or whatever
float offset = 1; //or whatever

//	Globals

int screenWidth = 640;
int screenHeight = 480;

float xmin;
float xmax;
float ymin;
float ymax;

int level = 5;

float sx = 0.0;
float sy = 0.0;

//	Function prototypes

void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void init();
void printInstructions();
void drawCircle(float r);
void drawX();
void drawO();
void drawSquare();
void drawLittleSquare();
void drawBoard();

/************************************************************************
*																		*
*	Function:	main													*
*																		*
************************************************************************/

int main(int argc, char* argv[])
{
	screenHeight = 480;
	screenWidth = 640;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - screenWidth)/2, (glutGet(GLUT_SCREEN_HEIGHT) - screenHeight)/2);//Center it on the screen
	glutCreateWindow("TicTacToe");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();
	return 0;
}

/************************************************************************
*																		*
*	Function:	init													*
*																		*
************************************************************************/

void init()
{
	glClearColor(0, 0.61, 0.38, 0.0);
	srand(time(0));
	printInstructions();
	return;
}

/************************************************************************
*																		*
*	Function:	printInstructions										*
*																		*
************************************************************************/

void printInstructions()
{
	cout << "Welcome to Resize" << endl << endl;

	cout << "Practice resizing the window." << endl;
	cout << "Drag the edges this way and that." << endl;
	cout << "Make sure that the square always occupies the middle 80%." << endl << endl;
	
	cout << "Then click the mouse here and there." << endl;
	cout << "Make sure that the little square appears at the mouse click, but only when the click is within the big square." << endl << endl;
	
	cout << "To quit, press ESC, then hit any key." << endl;

	return;
}

/************************************************************************
*																		*
*	Function:	display													*
*																		*
************************************************************************/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    drawBoard();

//	drawSquare();

//	glTranslatef(sx, sy, 0.0);
//	drawLittleSquare();

	glutSwapBuffers();
	return;
}

/************************************************************************
*																		*
*	Function:	reshape													*
*																		*
************************************************************************/

void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	
//	Compute the values of xmin, xmax, ymin, ymax




/*	if (w > 0 && h > 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(xmin, xmax, ymin, ymax);
		glViewport(0, 0, w, h);
		glutPostRedisplay();
	}
 */
	return;
}

/************************************************************************
*																		*
*	Function:	keyboard												*
*																		*
************************************************************************/

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'X': case 'x':

		//	Restart the game



			break;
		case 'O': case 'o':

		//	Restart the game and let O move



			break;
		case ESC:
			exit(0);
			break;
		default:
			break;
	}
	return;
}

/************************************************************************
*																		*
*	Function:	special													*
*																		*
************************************************************************/

void special(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			level = 1;
			break;
		case GLUT_KEY_F2:
			level = 2;
			break;
		case GLUT_KEY_F3:
			level = 3;
			break;
		case GLUT_KEY_F4:
			level = 4;
			break;
		case GLUT_KEY_F5:
			level = 5;
			break;
		default:
			break;
	}
	return;
}

/************************************************************************
*																		*
*	Function:	mouse													*
*																		*
************************************************************************/

void mouse(int button, int state, int x, int y)
{
//	Compute sx and sy in world coordinates to make the little square appear
//	where the mouse was clicked.

	return;
}

/************************************************************************
*																		*
*	Function:	drawBoard												*
*																		*
*	Purpose:	To draw the tic tac toe game board						*
*																		*
************************************************************************/

void drawBoard()
{

    if (screenHeight < screenWidth)
    {
        float diff = screenWidth * .15;
        BOARD_WIDTH = screenWidth - diff;
        offset = diff / 2;
    }
    else 
    {   float diff = screenHeight * .15;
        BOARD_WIDTH = screenHeight - diff;
        offset = diff / 2;
    }
    
    glColor3f(0.0f, 0.0f, 0.0f);
    
    
    glBegin(GL_POLYGON);
        glVertex2f(1, 1);
        glVertex2f(1, 50);
        glVertex2f(50, 50);
        glVertex2f(50, 1);
        glVertex2f(1,1);
	glEnd();
    /*
     glBegin(GL_LINES);
     glVertex2f(BOARD_WIDTH/3.0, 0.0);
     glVertex2f(BOARD_WIDTH/3.0, BOARD_WIDTH);
     
     glVertex2f(2.0*BOARD_WIDTH/3.0, 0.0);
     glVertex2f(2.0*BOARD_WIDTH/3.0, BOARD_WIDTH);
     
     glVertex2f(0.0, BOARD_WIDTH/3.0);
     glVertex2f(BOARD_WIDTH, BOARD_WIDTH/3.0);
     
     glVertex2f(0.0, 2.0*BOARD_WIDTH/3.0);
     glVertex2f(BOARD_WIDTH, 2.0*BOARD_WIDTH/3.0);
     
     glEnd();
     */


	return;
}

/************************************************************************
*																		*
*	Function:	drawCircle												*
*																		*
*	Purpose:	To draw a unit circle, centered at the origin			*
*																		*
************************************************************************/

void drawCircle(float r)
{
	float angle = 0.0;
	float dAngle = PI/20.0;
	glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < 40; i++)
		{
			glVertex2f(r*cos(angle), r*sin(angle));
			angle += dAngle;		
		}
	glEnd();
	return;
}

/************************************************************************
*																		*
*	Function:	drawX													*
*																		*
*	Purpose:	To draw a big X											*
*																		*
************************************************************************/

void drawX()
{
//	Draw an X

	return;
}

/************************************************************************
*																		*
*	Function:	drawO													*
*																		*
*	Purpose:	To draw a big O											*
*																		*
************************************************************************/

void drawO()
{
//	Draw an O

	return;
}


/************************************************************************
*																		*
*	Function:	drawSquare												*
*																		*
*	Purpose:	To draw a big square									*
*																		*
************************************************************************/

void drawSquare()
{
//	Draw a big square, divided into 9 little squares

	glColor3f(1, 0.62, 0.0);
    
    ///(screenWidth-(screenWidth *.15))/2
    int WOffset = (screenWidth-(screenWidth *.15))/2;
    int HOffset = (screenHeight-(screenHeight *.15))/2;
    cout << WOffset << endl << HOffset << endl;
    
 //   glRectf(WOffset,screenHeight- HOffset,screenWidth - WOffset, HOffset);

	glBegin(GL_LINE_LOOP);
		glVertex2f(WOffset, HOffset);
		glVertex2f(WOffset, screenHeight- HOffset);
		glVertex2f(screenWidth - WOffset, screenHeight- HOffset);
		glVertex2f(screenWidth - WOffset, HOffset);
	glEnd();
/*
	glBegin(GL_LINES);
		glVertex2f(BOARD_WIDTH/3.0, 0.0);
		glVertex2f(BOARD_WIDTH/3.0, BOARD_WIDTH);

		glVertex2f(2.0*BOARD_WIDTH/3.0, 0.0);
		glVertex2f(2.0*BOARD_WIDTH/3.0, BOARD_WIDTH);

		glVertex2f(0.0, BOARD_WIDTH/3.0);
		glVertex2f(BOARD_WIDTH, BOARD_WIDTH/3.0);

		glVertex2f(0.0, 2.0*BOARD_WIDTH/3.0);
		glVertex2f(BOARD_WIDTH, 2.0*BOARD_WIDTH/3.0);

	glEnd();
 
 */

	return;
}

/************************************************************************
*																		*
*	Function:	drawLittleSquare										*
*																		*
*	Purpose:	To draw a little square									*
*																		*
************************************************************************/

void drawLittleSquare()
{
//	Draw a little red square

	float side = BOARD_WIDTH/10.0;

	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0, 0.0);
		glVertex2f(side, 0.0);
		glVertex2f(side, side);
		glVertex2f(0.0, side);
	glEnd();

	return;
}

