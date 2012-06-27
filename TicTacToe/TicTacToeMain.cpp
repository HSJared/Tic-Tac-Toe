/****************************************************************************
 *																			*
 *	Program:	TicTacToe.cpp												*
 *																			*
 *	Author:		Jared Christian                                             *
 *																			*
 *	Date:		Oct 7, 2011                                                 *
 *																			*
 *	Purpose:	This program will play a game of Tic Tac Toe.				*
 *																			*
 ****************************************************************************/

//	Include header files

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
//if using windows: #include <gl/glut.h>
#include <cmath>
#include "TTTLogic.h"
#include "set.h"

using namespace std;

//	Constants

const float PI = 3.14159265358979323;
const int ESC = 27;

//The Drawing Area
float xmin;
float xmax;
float ymin;
float ymax;

float sx = 0.0;
float sy = 0.0;

//Screen Size
int screenWidth = 640;
int screenHeight = 480;

/*****These are not defined here on purpose*******/

// Globals for dynamically drawing the Game Board
float BOARD_WIDTH = 120;
float GRID_WIDTH = BOARD_WIDTH / 16;
float CELL_PADDING = GRID_WIDTH / 4;
float CELL_WIDTH = (BOARD_WIDTH - (GRID_WIDTH * 2))/3;


//The Sides of the Grid
float FIRST_GRID_CLOSE_SIDE; //Near side X or Y of Grid closest to 0
float FIRST_GRID_FAR_SIDE; // Far side X or Y of Grid Closest
float SECOND_GRID_CLOSE_SIDE; //etc
float SECOND_GRID_FAR_SIDE; //etc

float EQAUL_SIDES_OF_Isosceles_Triangle; //This is the Corned Edge of the X and of the Diagonal Winning Lines. It's used 16 times. No need to recompute

/*****These are not defined here on purpose*******/

//The Two Players
Player Xplayer;
Player Oplayer;
bool twoplayer; //For the two player mode. This disables the computer control logic.

//The Tracking the Current Game
int level = 5; //Current level

char currentPlayer; //Who's turn it is.
char computer; //which one the computer is
bool gameOver; //is the game done? This is its own varible because it seems silly to call isBoardFilled in every mouse function call. A bool will do.


//	Globals
// float offset = 1; //or whatever


//	The Color struct and the 8 colors

struct Color
{
	float r;
	float g;
	float b;
} 
color[8] =
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

//Main/Needed Functions
void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);
void init();
void printInstructions();
void mouse(int button, int state, int x, int y);
void handleCellClick(int cell);
void special(int key, int x, int y);


//Drawing the Board
void drawBoard();
void drawGrid();
void drawCurrentGameState();
void inspectWinningState();
void drawWinningLine(int winningConfig);
void drawHorizontalWinningLine(int winningConfig);
void drawVerticalWinningLine(int winningConfig);
void drawDiagonalWinningLine(int winningConfig);

//Drawing the shapes on the Board
void drawX();
void drawO();
void drawCircle(float r);

//Computing the Size of the Board and Global Varibles
void determineVariables();

//Using Global Varibles to handle mouse events
int locateCell(int workingX, int workingY);
int findRow(int workingY);
int getXCoordinateOfCell(int cell);
int getYCoordinateOfCell(int cell);

//Game Logic
void clearGame (char firstplayer = 'x');
bool isCellOpen(int cell);
void computerMove();

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
	glutInitWindowPosition(100, 150);
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
    //	srand(time(0));
	printInstructions();
    currentPlayer = 'x';
    computer = 'o';
    gameOver = false;
    twoplayer = false;
    level = 1;
	return;
}

/************************************************************************
 *																		*
 *	Function:	printInstructions										*
 *																		*
 ************************************************************************/

void printInstructions()
{
	cout << "Welcome to TicTacToe" << endl << endl;
    
	cout << "Press O or X to start a new game as player O or X." << endl;
    cout << "Press 1-5 in order to play the computer at the desired level " << endl;
	cout << "Press T to toggle on Two Player Mode. " << endl;
	
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
    
	glTranslatef(sx, sy, 0.0);
    // glTranslatef(xmin, ymin, 0.0);
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
    
    determineVariables();
    
    
	if (w > 0 && h > 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(xmin, xmax, ymin, ymax); //I made them - here because I used the positive values of xmin, ymin elsewhere
		glViewport(0, 0, w, h);
		glutPostRedisplay();
	}
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
            clearGame('x');
            //	Restart the game
			break;
		case 'O': case 'o':
            clearGame('o');
            //	Restart the game and let O move
        case '1':
            level = 1;
            break;
        case '2':
            level = 2;
            break;
        case '3':
            level = 3;
            break;
        case '4':
            level = 4;
            break;
        case '5':
            level = 5;
            break;
        case 'c': case 'C':                     //Clear the Game
            clearGame('x');
            computerMove();
            glutPostRedisplay();
            break;
        case 't': case 'T':                     //Toggle Two Player Mode
            twoplayer = !twoplayer;
            clearGame('x');
            glutPostRedisplay();
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
            clearGame(currentPlayer);
			break;
		case GLUT_KEY_F2:
			level = 2;
            clearGame(currentPlayer);
			break;
		case GLUT_KEY_F3:
			level = 3;
            clearGame(currentPlayer);
			break;
		case GLUT_KEY_F4:
			level = 4;
            clearGame(currentPlayer);
			break;
		case GLUT_KEY_F5:
			level = 5;
            clearGame(currentPlayer);
			break;
		default:
			break;
	}
	return;
}

/****************************************************************************
 *																			*
 *	Function:	clearGame                                                   *
 *																			*
 *	Purpose:	To clear the game and start over                            *
 *																			*
 ****************************************************************************/

void clearGame(char firstplayer)
{
    
    // 1. Create Empty Players
    Xplayer.makeEmpty();
    Oplayer.makeEmpty();
    
    // 2. Set needed game logic values
    gameOver = false;
    if (firstplayer != 'o') {
        currentPlayer = 'x';
        computer = 'o';
    }
    else
    {
        currentPlayer = 'o';
        computer = 'x';
    }
    
    // 3. Refresh Display
    glutPostRedisplay();
    
}

/****************************************************************************
 *																			*
 *	Function:	determineVaribles                                           *
 *																			*
 *	Purpose:	To compute the global varibles that allow the board         *
 *              to be dynamically drawn                                     *
 *																			*
 ****************************************************************************/

void determineVariables()
{
    //1. Get Board Width
    if (screenHeight <= screenWidth)
        BOARD_WIDTH = screenHeight * 0.8;
    else BOARD_WIDTH = screenWidth * 0.8;
    
    //2. Define Coodinate System
    xmin = -(screenWidth - BOARD_WIDTH) / 2;
    xmax = screenWidth - ((screenWidth - BOARD_WIDTH) / 2);
    ymin = -(screenHeight - BOARD_WIDTH) / 2;
    ymax = screenHeight - ((screenHeight - BOARD_WIDTH) / 2);
    
    /************
     *DEBUG_CODE*
     **********
    // cout << "Size: "<< screenWidth << "x" << screenHeight << " BOARD_WIDTH: " << BOARD_WIDTH << endl;
    // cout << "xmin: " << xmin << " xmax: "; 
    // cout << xmax << " ymin: "; 
    // cout << ymin << " ymax: ";
    // cout << ymax << endl; */
    
    //3. Compute Basic Board Paramaters
    GRID_WIDTH = BOARD_WIDTH / 16; //Everything depends on this proportion. 
    CELL_WIDTH = (BOARD_WIDTH - (GRID_WIDTH * 2))/3;
    CELL_PADDING = GRID_WIDTH / 4;
    
    //4. Compute Locations of Grids to reference. Close refers to side closer to origin. Far is the other side.
    FIRST_GRID_CLOSE_SIDE = CELL_WIDTH; // Origin + Cell Width
    FIRST_GRID_FAR_SIDE = CELL_WIDTH + GRID_WIDTH; // // Origin + Cell Width + Width of First Grid
    SECOND_GRID_CLOSE_SIDE = 2.0 * CELL_WIDTH + GRID_WIDTH; // Origin + Width of two rows + width of first grid
    SECOND_GRID_FAR_SIDE = 2.0 * CELL_WIDTH + 2.0 * GRID_WIDTH; //etc
    
    
    //5. Compute the A and B side of the Right Triangle for the Cornered Edges of X and Diagonal Winning Lines
    EQAUL_SIDES_OF_Isosceles_Triangle = GRID_WIDTH / sqrt(2);
    
    /*  Note: I make the varibles in steps 4 and 5 global because they are used multiple times. It's better to compute once
     *  if you are computing the same thing over and over. It also makes it much easier to have one authrative source on 
     *  the board variables. My mouse and drawing functions use all these.
     */ 
    
    
}

/************************************************************************
 *																		*
 *	Function:	mouse													*
 *																		*
 ************************************************************************/

void mouse(int button, int state, int x, int y)
{
    
    // 1. Don't go anything if it's not a left click, it's a mouse release, or the game is already over
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver) 
    {
        
        
        /************
         *DEBUG_CODE*
         ***********/
        // cout << "Button: " << button << endl;
        //   time_t curr=time(0);
        // cout << "current time is: " << curr <<endl;
        
        //2. Find where the mouse was clicked in GL Coordinates
        int glycoordinate = screenHeight - y;
        
        /************
         *DEBUG_CODE*
         ***********/
        // cout  << "x: " << x << " y: " << glycoordinate << endl;
        
        //3. Locate which cell (if any) was clicked. int cell is 9 if not in a cell.
        int cell = locateCell(x,glycoordinate);
        
        /************
         *DEBUG_CODE*
         ***********/
        // cout    << "Cell #: " << cell << endl;
        
        //4. Based on what cell was clicked, do what needs to be done
        handleCellClick(cell);
        
    }
    
	return;
}

/****************************************************************************
 *																			*
 *	Function:	handleCellClick                                             *
 *																			*
 *	Purpose:	To perform game logic for a clicked cell                    *
 *																			*
 ****************************************************************************/

void handleCellClick(int cell)
{
    //1. 9 = Grid Line or not on board. If so, don't do anything.
    if (cell == 9)
        return;
    //2. Check if cell is open. If not, don't do anything
    if (isCellOpen(cell)) {
        
        //A. Move X if X is playing
        if (currentPlayer == 'x')
        {
            Xplayer += cell;
            currentPlayer = 'o';
        }
        
        //B. Move O if O is playing
        else
        {
            Oplayer += cell;
            currentPlayer = 'x';
        }
        inspectWinningState();
        // C. IMPORTANT. Check to see if board is filled. If so mark as so, so that mouse clicks are ignored.
        if (isBoardFilled(Xplayer, Oplayer) || hasWon(Xplayer) || hasWon(Oplayer))
        {
            gameOver = true;
        }
        else if (!gameOver) computerMove();
        
        //D. Redraw the board to display updated info
        glutPostRedisplay();
    }
    //else  cout << "Cell # " << cell << " is NOT open!" << endl;
    
}

/****************************************************************************
 *																			*
 *	Function:	isCellOpen                                             *
 *																			*
 *	Purpose:	To check if a cell is open                                  *
 *																			*
 ****************************************************************************/

bool isCellOpen(int cell)
{   
    //I figured it would make more sense to abstract away two calls to isOccuiped.
    if (isOccupied(Xplayer, cell) || isOccupied(Oplayer, cell)) {
        return false;
    }
    else return true;
}

/****************************************************************************
 *																			*
 *	Function:	computerMove                                                *
 *																			*
 *	Purpose:	to make the computers move                                  *
 *																			*
 ****************************************************************************/

void computerMove()
{
    // 1. Only play if two player mode is off
    if (!twoplayer) {
        
        //2. Play
        switch (currentPlayer) {
            case 'o':
                Oplayer += nextMove(Xplayer, Oplayer,level); //Second Parameter is Computer
                currentPlayer = 'x';
                break;
            case 'x':
                Xplayer += nextMove(Oplayer, Xplayer, level); //Second Parameter is Computer
                currentPlayer = 'o';
            default:
                break;
        }
    }
}

/****************************************************************************
 *																			*
 *	Function:	locateCell                                                  *
 *																			*
 *	Purpose:	using the GL coordinates of a click, to return the clicked  *
 *              cell                                                        *
 *																			*
 ****************************************************************************/

int locateCell(int x, int y)
{
    //Return Values: 0-8 What Cell was clicked
    //9 = didn't click on cell
    
    // 1.   Crop out the parts that are not in the coordinate system. This allows the program to 
    //      find the cell using the globals so everything is consistant.
    
    int orientatedX = x + (int)xmin; //Round down to give user benefit of doubt. 
    int orientatedY = y + (int)ymin; //Again, round down. No need to make field of clicking smaller by one pixel because of rounding
    
    /************
     *DEBUG_CODE*
     ***********/
    //    // cout << "Working X: " << workingX <<" WorkingY: " << workingY << endl;
    
    
    //2. Eliminate Clicks that are not on the game board. 
    if (orientatedX < 0 || orientatedX > BOARD_WIDTH || orientatedY < 0 || orientatedY > BOARD_WIDTH) {
        return 9;
    }
    
    int column;
    int row = findRow(orientatedY);
    if (orientatedX < FIRST_GRID_CLOSE_SIDE)
    {    column = 0;
        return 3*row + column;
    }
    else if (orientatedX > FIRST_GRID_FAR_SIDE && orientatedX < SECOND_GRID_CLOSE_SIDE)
    {    column = 1; 
        return 3*row + column;
    }
    else if (orientatedX > SECOND_GRID_FAR_SIDE && orientatedX < BOARD_WIDTH)
    {    column = 2;
        return 3*row + column;
    }
    
    return 9; //if it's not in a column, it's in a non-clickable area.
}

/****************************************************************************
 *																			*
 *	Function:	locateCell                                                  *
 *																			*
 *	Purpose:	To find which row the Y coordinate is in                    *
 *																			*
 ****************************************************************************/

int findRow(int y)
{   //If it's smaller than first grid, it's in first row
    if (y < FIRST_GRID_CLOSE_SIDE) 
    {
        return 0;
    }
    
    //Between First and Second Gird
    else if (y > FIRST_GRID_FAR_SIDE && y < SECOND_GRID_CLOSE_SIDE)
    {
        return 1;
    }
    
    //Etc
    else if (y > SECOND_GRID_FAR_SIDE && y < BOARD_WIDTH)
    {
        return 2;
    }
    //If all else fails, it's a gridline
    return 3;
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
    glColor3f(1, 1, 0.4); //Banana Color
    
    drawGrid(); //Draw the blank grid
    
    drawCurrentGameState(); //draw the completed moves and winning lines
    
	return;
}

/************************************************************************
 *																		*
 *	Function:	drawGrid												*
 *																		*
 *	Purpose:	To draw the blank tic tac toe game board				*
 *																		*
 ************************************************************************/

void drawGrid()
{
    glColor3f(1, 1, 0.4); //Banana Color
    
    //First Vertical Line
	glBegin(GL_TRIANGLE_FAN);
        glVertex2f(FIRST_GRID_CLOSE_SIDE, 0);
        glVertex2f(FIRST_GRID_CLOSE_SIDE, BOARD_WIDTH);
        glVertex2f(FIRST_GRID_FAR_SIDE, BOARD_WIDTH);
        glVertex2f(FIRST_GRID_FAR_SIDE, 0);
    glEnd();
    
    //Second Vertical Line
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(SECOND_GRID_CLOSE_SIDE, 0);
        glVertex2f(SECOND_GRID_CLOSE_SIDE, BOARD_WIDTH);
        glVertex2f(SECOND_GRID_FAR_SIDE, BOARD_WIDTH);
        glVertex2f(SECOND_GRID_FAR_SIDE, 0);
    glEnd();
    
    //Lower Horizontal Line
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, FIRST_GRID_CLOSE_SIDE);
        glVertex2f(0,FIRST_GRID_FAR_SIDE);
        glVertex2f(BOARD_WIDTH, FIRST_GRID_FAR_SIDE);
        glVertex2f(BOARD_WIDTH, FIRST_GRID_CLOSE_SIDE);
    glEnd();
    
    //Higher Horizontal Line
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,SECOND_GRID_CLOSE_SIDE);
        glVertex2f(0,SECOND_GRID_FAR_SIDE);
        glVertex2f(BOARD_WIDTH,SECOND_GRID_FAR_SIDE);
        glVertex2f(BOARD_WIDTH,SECOND_GRID_CLOSE_SIDE);
    glEnd();
}
/************************************************************************
 *																		*
 *	Function:	drawCurrentGameState									*
 *																		*
 *	Purpose:	To draw the player moves and winning lines              *
 *																		*
 ************************************************************************/

void drawCurrentGameState()
{
    //1. Draw Marks on Board
    for (int i = 0; i < 9; i++) 
    {
        //A. Draw Xs
        if (isOccupied(Xplayer, i)) 
        {
            // i. get pos to draw X at
            int xpos = getXCoordinateOfCell(i);
            int ypos = getYCoordinateOfCell(i);
            
            /************
             *DEBUG_CODE*
             ***********/
            // cout << "Cell # " << i << " DrawX::::X: " << xpos << " Y: " << ypos << endl;
            
            // ii. Draw the X
            glTranslatef(xpos, ypos, 0);
            drawX();
            glTranslatef(-xpos, -ypos, 0);
        }
        
        //B. Draw Os
        else if (isOccupied(Oplayer, i))
        {
            // i. Since the circle is drawn with the origin in middle, get offset to center of cell. 
            int offset = (CELL_WIDTH - (CELL_PADDING * 2)) / 2;
            
            // ii. get pos to draw O at, take offset into account
            int xpos = getXCoordinateOfCell(i) + offset;
            int ypos = getYCoordinateOfCell(i) + offset;
            // cout << "Cell # " << i << " DrawO::::X: " << xpos << " Y: " << ypos << endl;
            
            // iii. draw the O
            glTranslatef(xpos, ypos, 0);
            drawO();
            glTranslatef(-xpos, -ypos, 0);
            
        }
    }
    
    // 2. Draw the winning lines if needed
    inspectWinningState();
}

/************************************************************************
 *																		*
 *	Function:	inspectWinningState                                     *
 *																		*
 *	Purpose:	check to see if game is won and draw lines if so        *
 *																		*
 ************************************************************************/

void inspectWinningState()
{   
    //Since O is the default computer player, makes sense to check to see if it won first
    if (hasWon(Oplayer)) {
        gameOver = true;
        int theWinningConfig = winningConfig(Oplayer);
        drawWinningLine(theWinningConfig);
    }
    
    // If not, check X
    else if (hasWon(Xplayer))
    {
        gameOver = true;
        int theWinningConfig = winningConfig(Xplayer);
        drawWinningLine(theWinningConfig);
    }
    
    return;
    
}

/************************************************************************
 *																		*
 *	Function:	drawWinningLine                                         *
 *																		*
 *	Purpose:	draw the winning line for whoever won                   *
 *																		*
 ************************************************************************/
void drawWinningLine(int winningConfig)
{
    int temp = winningConfig / 3; //It'll round down.
    
    //1. Since ints truncate 0-2 is Horizontal,3-5 is Vert, and 6 and 7 are diagonal, this can be done.
    
    switch (temp) {
        case 0:
            drawHorizontalWinningLine(winningConfig);
            break;
        case 1:
            drawVerticalWinningLine(winningConfig);
            break;
        case 2:
            drawDiagonalWinningLine(winningConfig);
            break;
        default:
            break;
    }
    
}

/************************************************************************
 *																		*
 *	Function:	drawHorizontalWinningLine                               *
 *																		*
 *	Purpose:	draw the horizontal winning lines                       *
 *																		*
 ************************************************************************/

void drawHorizontalWinningLine(int winningConfig)
{
    glColor3f(0.76, 0.0078, 0.2); //Pysch Red
    
    switch (winningConfig) {
        case 0:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0, (CELL_WIDTH/2) - (GRID_WIDTH/2));
                glVertex2f(0, (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, (CELL_WIDTH/2) - (GRID_WIDTH/2));
            glEnd();
            break;
        case 1:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0, FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2));
                glVertex2f(0, FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2));
            glEnd();
            break;
        case 2:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0, SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2));
                glVertex2f(0, SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2));
                glVertex2f(BOARD_WIDTH, SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2));
            glEnd();
            break;
        default:
            break;
    }
}

/************************************************************************
 *																		*
 *	Function:	drawVerticalWinningLine                                 *
 *																		*
 *	Purpose:	draw the vertical winning lines                         *
 *																		*
 ************************************************************************/

void drawVerticalWinningLine(int winningConfig)
{
    glColor3f(0.76, 0.0078, 0.2); //Pysch Red
    
    switch (winningConfig) {
        case 3:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f((CELL_WIDTH/2) - (GRID_WIDTH/2), 0);
                glVertex2f((CELL_WIDTH/2) - (GRID_WIDTH/2), BOARD_WIDTH);
                glVertex2f((CELL_WIDTH/2) + (GRID_WIDTH/2),BOARD_WIDTH);
                glVertex2f((CELL_WIDTH/2) + (GRID_WIDTH/2),0);
            glEnd();
            break;
        case 4:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2), 0);
                glVertex2f(FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2), BOARD_WIDTH);
                glVertex2f(FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2),BOARD_WIDTH);
                glVertex2f(FIRST_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2),0);
            glEnd();
            break;
        case 5:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2), 0);
                glVertex2f(SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) - (GRID_WIDTH/2), BOARD_WIDTH);
                glVertex2f(SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2),BOARD_WIDTH);
                glVertex2f(SECOND_GRID_FAR_SIDE + (CELL_WIDTH/2) + (GRID_WIDTH/2),0);
            glEnd();
            break;
        default:
            break;
    }
    
}

/************************************************************************
 *																		*
 *	Function:	drawDiagonalWinningLine                                 *
 *																		*
 *	Purpose:	draw the diagonal winning lines                         *
 *																		*
 ************************************************************************/


void drawDiagonalWinningLine(int winningConfig)
{
    glColor3f(0.76, 0.0078, 0.2); //Pysch Red
    
    switch (winningConfig) {
        case 6:
            /**Foreward Slash**/
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0,BOARD_WIDTH);
                glVertex2f(EQAUL_SIDES_OF_Isosceles_Triangle,BOARD_WIDTH);
                glVertex2f(BOARD_WIDTH, EQAUL_SIDES_OF_Isosceles_Triangle);
                glVertex2f(BOARD_WIDTH, 0);
                glVertex2f(BOARD_WIDTH - EQAUL_SIDES_OF_Isosceles_Triangle, 0);
                glVertex2f(0,BOARD_WIDTH - EQAUL_SIDES_OF_Isosceles_Triangle);
            glEnd();
            break;
            
            /**Backward Slash**/
        case 7:
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0,0);
                glVertex2f(0,EQAUL_SIDES_OF_Isosceles_Triangle);
                glVertex2f(BOARD_WIDTH - EQAUL_SIDES_OF_Isosceles_Triangle, BOARD_WIDTH);
                glVertex2f(BOARD_WIDTH, BOARD_WIDTH);
                glVertex2f(BOARD_WIDTH, BOARD_WIDTH - EQAUL_SIDES_OF_Isosceles_Triangle);
                glVertex2f(EQAUL_SIDES_OF_Isosceles_Triangle,0);
            glEnd();
            break;
        default:
            break;
    }
}

/************************************************************************
 *																		*
 *	Function:	getXCoordinateOfCell                                    *
 *																		*
 *	Purpose:	get the x coordinate of a cell inorder to draw X or O   *
 *																		*
 ************************************************************************/


int getXCoordinateOfCell(int cell)
{
    int temp = cell % 3; //0 = First Row, 1 = Second, 2 = Third
    
    switch (temp) {
        case 0:
            return CELL_PADDING;
            break;
        case 1:
            return FIRST_GRID_FAR_SIDE + CELL_PADDING;
            break;
        case 2:
            return SECOND_GRID_FAR_SIDE + CELL_PADDING;
            break;
        default:
            break;
    }
    
    return -1;
}

/************************************************************************
 *																		*
 *	Function:	getYCoordinateOfCell                                    *
 *																		*
 *	Purpose:	get the y coordinate of a cell inorder to draw X or O   *
 *																		*
 ************************************************************************/

int getYCoordinateOfCell(int cell)
{
    int temp = cell/3; //Since it's an int, it will round down. 1/3 = 0; Bottom Row will be 0, Middle 1, and Top 2.
    switch (temp) {
        case 0:
            return CELL_PADDING;
            break;
        case 1:
            return FIRST_GRID_FAR_SIDE + CELL_PADDING;
            break;
        case 2:
            return SECOND_GRID_FAR_SIDE + CELL_PADDING;
            break;
        default:
            break;
    }
    return -1;
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
    //Find the offset for the cornered edges
    glColor3f(0.588, 0.87, 0.819);
    
    //   / of X
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        glVertex2f(0,EQAUL_SIDES_OF_Isosceles_Triangle);
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2) - EQAUL_SIDES_OF_Isosceles_Triangle, CELL_WIDTH - (CELL_PADDING * 2));
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2), CELL_WIDTH - (CELL_PADDING * 2));
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2), CELL_WIDTH - (CELL_PADDING * 2) - EQAUL_SIDES_OF_Isosceles_Triangle);
        glVertex2f(EQAUL_SIDES_OF_Isosceles_Triangle,0);
    glEnd();
    
    // \ of X
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,CELL_WIDTH - (CELL_PADDING * 2));
        glVertex2f(0,CELL_WIDTH - (CELL_PADDING * 2) - EQAUL_SIDES_OF_Isosceles_Triangle);
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2) - EQAUL_SIDES_OF_Isosceles_Triangle,0);
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2),0);
        glVertex2f(CELL_WIDTH - (CELL_PADDING * 2),EQAUL_SIDES_OF_Isosceles_Triangle);
        glVertex2f(EQAUL_SIDES_OF_Isosceles_Triangle,CELL_WIDTH - (CELL_PADDING * 2));
    glEnd();
    
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
    int radius = (CELL_WIDTH - (CELL_PADDING * 2))/2;
    glColor3f(1, 0.6, 0.4);
    drawCircle(radius);
    glColor3f(0, 0.61, 0.38);
    drawCircle(radius - GRID_WIDTH);
    
	return;
}
