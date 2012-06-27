/****************************************************************************
 *																			*
 *	Program:	TicTacToe.cpp												*
 *																			*
 *	Author:		Jared Christian                                             *
 *																			*
 *	Date:		Aug 19, 2011												*
 *																			*
 *	Purpose:	This program will play a game of Tic Tac Toe.				*
 *																			*
 ****************************************************************************/

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

float xmin;
float xmax;
float ymin;
float ymax;
float CELL_WIDTH;
float GRID_WIDTH;
float CELL_PADDING;

//	Globals

int screenWidth = 640;
int screenHeight = 480;

float BOARD_WIDTH = 120;  // Or whatever
float offset = 1; //or whatever


//	The Color struct and the 8 colors

struct Color
{
	float r;
	float g;
	float b;
} color[8] =
{
	{1.0, 1.0, 1.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.6, 0.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.5, 0.0, 1.0},
	{0.0, 0.0, 0.0}
};

//	Function prototypes

void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);
void init();
void printInstructions();
void drawBoard();
void determineminmax();

/************************************************************************
 *																		*
 *	Function:	main													*
 *																		*
 ************************************************************************/

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - screenWidth)/2, (glutGet(GLUT_SCREEN_HEIGHT) - screenHeight)/2);//Center it on the screen
	glutCreateWindow("TicTacToe");
    
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
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
	cout << "Lecture 1 Demo 1: The Graphics Window" << endl << endl;
    
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
	//Init Stuff
    glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
        
    determineminmax();
    
    glTranslatef(xmin,ymin,0.0);
    
  /*  glBegin(GL_LINE_LOOP);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, BOARD_WIDTH);
        glVertex2f(BOARD_WIDTH, BOARD_WIDTH);
        glVertex2f(BOARD_WIDTH, 0.0);
	glEnd();

    //Vertical Lines
    glBegin(GL_POLYGON);
        glVertex2f(CELL_WIDTH, 0);
        glVertex2f(CELL_WIDTH, BOARD_WIDTH);
        glVertex2f(CELL_WIDTH + GRID_WIDTH, BOARD_WIDTH);
        glVertex2f(CELL_WIDTH + GRID_WIDTH, 0);
	glEnd(); */
    
    drawBoard();
    
    glutSwapBuffers();

	return;
}

void drawBoard()
{
    glColor3f(1,0.4,0);  //Safety Orange
    
    
 glTranslatef(0.0,0.0,0.0);
 
 glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin );
    glVertex2f(xmin, ymin + BOARD_WIDTH);
    glVertex2f(xmin + BOARD_WIDTH, ymin + BOARD_WIDTH);
    glVertex2f(xmin + BOARD_WIDTH, ymin);
/*    glVertex2f(xmin, 0.0);
    glVertex2f(0.0, BOARD_WIDTH);
    glVertex2f(BOARD_WIDTH, BOARD_WIDTH);
    glVertex2f(BOARD_WIDTH, 0.0); */
 glEnd();
 
 //Vertical Lines
/* glBegin(GL_POLYGON);
    glVertex2f(CELL_WIDTH, ymin);
    glVertex2f(CELL_WIDTH, BOARD_WIDTH);
    glVertex2f(CELL_WIDTH + GRID_WIDTH, BOARD_WIDTH);
    glVertex2f(CELL_WIDTH + GRID_WIDTH, ymin);
 glEnd(); */
 
/*  glBegin(GL_POLYGON);
        glVertex2f(GRID_WIDTH + CELL_WIDTH * 2, ymin);
        glVertex2f(GRID_WIDTH + CELL_WIDTH * 2, BOARD_WIDTH);
        glVertex2f(CELL_WIDTH * 2 + GRID_WIDTH * 2, BOARD_WIDTH);
        glVertex2f(CELL_WIDTH * 2 + GRID_WIDTH * 2, ymin);
 glEnd();
 
 //Horizontal Lines
 
 glBegin(GL_POLYGON);
        glVertex2f(0, CELL_WIDTH);
        glVertex2f(0, CELL_WIDTH + GRID_WIDTH);
        glVertex2f(BOARD_WIDTH, CELL_WIDTH + GRID_WIDTH);
        glVertex2f(BOARD_WIDTH, CELL_WIDTH);
 glEnd();
 
 glBegin(GL_POLYGON);
        glVertex2f(0, GRID_WIDTH + CELL_WIDTH * 2);
        glVertex2f(0, CELL_WIDTH * 2 + GRID_WIDTH * 2);
        glVertex2f(BOARD_WIDTH, CELL_WIDTH * 2 + GRID_WIDTH * 2);
        glVertex2f(BOARD_WIDTH, GRID_WIDTH + CELL_WIDTH * 2);
 glEnd(); 
 
 */
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
    
    determineminmax();

    
	if (w > 0 && h > 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(xmin, xmax, ymin, ymax);
		glViewport(0, 0, w, h);
		glutPostRedisplay();
	}
    
	return;
}

void determineminmax()
{
    if (screenHeight <= screenWidth)
    {
        BOARD_WIDTH = screenHeight * 0.8;
        cout << "Size: "<< screenWidth << "x" << screenHeight << " BOARD_WIDTH: " << BOARD_WIDTH << endl;
        xmin = (screenWidth - BOARD_WIDTH) / 2;
        xmax = xmin + BOARD_WIDTH;
        ymin = screenHeight * 0.1;
        ymax = screenHeight * 0.9;
        cout << "xmin: " << xmin << " xmax: "; 
        cout << xmax << " ymin: "; 
        cout << ymin << " ymax: ";
        cout << ymax << endl;
        
    }
    else 
    {   
        BOARD_WIDTH = screenWidth * 0.8;
        cout << "Size: "<< screenWidth << "x" << screenHeight << " BOARD_WIDTH: " << BOARD_WIDTH << endl;
        xmin = screenWidth * 0.1;
        xmax = xmin + BOARD_WIDTH;
        ymin = (screenHeight - BOARD_WIDTH) / 2;
        ymax = ymin + BOARD_WIDTH;
        cout << "xmin: " << xmin << " xmax: "; 
        cout << xmax << " ymin: "; 
        cout << ymin << " ymax: ";
        cout << ymax << endl;
        
    }
    
    GRID_WIDTH = BOARD_WIDTH / 16;
    CELL_WIDTH = (BOARD_WIDTH - (GRID_WIDTH * 2)) /2;
    CELL_PADDING = GRID_WIDTH / 4;
    
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
		case ESC:
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
	return;
}