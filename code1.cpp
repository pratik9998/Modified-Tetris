#include <GL/glut.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include<bits/stdc++.h>
using namespace std;
#define B 0  // Vanish zone height
#define M 11 // Field height
#define N 22 // Field width

const float S = 0.05; // Square size

/* Figure matrix
 * 0 1 2 3
 * 4 5 6 7
 */
int score = 0;

const int figures[7][4] = {
	{4, 5, 6, 7}, // I
	{1, 2, 5, 6}, // O
	{4, 5, 6, 1}, // T
	{0, 1, 5, 6}, // S
	{4, 5, 1, 2}, // Z
	{0, 4, 5, 6}, // J
	{4, 5, 6, 2}, // L
};

const float colors[7][3] = {
	{0.000, 0.545, 0.545}, // Cyan
	{1.000, 0.843, 0.000}, // Yellow
	{0.545, 0.000, 0.545}, // Purple
	{0.196, 0.804, 0.196}, // Green
	{0.698, 0.133, 0.133}, // Red
	{0.098, 0.098, 0.498}, // Blue
	{1.000, 0.549, 0.000}, // Orange
};

struct point {
	int x, y;
} a[4], b[4], c[4];

int delay = 600;
int field[M][N] = {0};
int shape, shapeNext;

int gameOver = 0;

int check(void) {
    for (int i = 0; i < 4; i++) {
        // Ensure the block stays within the horizontal bounds of the field
        if (a[i].x < 0 || a[i].x >= N) return 0;

        // Ensure the block stays within the vertical bounds of the field, including above the vanish zone
        if (a[i].y < 0 || a[i].y >= M) return 0;

        // Ensure the block does not collide with existing blocks
        if (field[a[i].y][a[i].x]) return 0;
    }
    return 1;
}



void generateFigure(void) {
	// Randomly choose a figure
	shapeNext = rand() % 7;

	// Generate the figure
	for (int i = 0; i < 4; i++) {
		c[i].x = figures[shapeNext][i] % 4 +  10  ;
		c[i].y = figures[shapeNext][i] / 4;
	}
}

void spawnFigure(void) {
    shape = shapeNext;

    for (int i = 0; i < 4; i++) {
        a[i].x = figures[shape][i] % 4 + N - 4; // Start near the right edge
        a[i].y = figures[shape][i] / 4 + M * 0.5 - 2; // Center vertically
    }

    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x -= 1; // Move one square left to start
    }

    if (!check()) {
        gameOver = 1;
    }

    generateFigure();
}


void drawText(float x, float y, string s, float Ox, float Oy, void *font) {
  if (font == NULL) font = GLUT_BITMAP_TIMES_ROMAN_24;
  //GLUT_BITMAP_TIMES_ROMAN_24
  //GLUT_BITMAP_HELVETICA_18
  //GLUT_BITMAP_9_BY_15
  
  // Offset the text
  y = -1.0 + y + Oy;
  x = -1.0 + x + Ox;

  // Set the position
  glRasterPos2f(x, y);
  
  int len = s.length();
  for(int i=0;i<len;i++){
  	glutBitmapCharacter(font, s[i]);
  }
}

void drawScore(float x, float y, int num, float Ox, float Oy, void *font) {
  if (font == NULL) font = GLUT_BITMAP_TIMES_ROMAN_24;
  
  string s = "";
  if(num == 0) s += '0';
  while(num > 0){
  	int d = num%10;
  	num /= 10;
  	s += (char)(d+'0');
  }

  int len = s.length();
  for(int i=0;i<len/2;i++) swap(s[i],s[len-i-1]);
  
  y = -1.0 + y + Oy;
  x = -1.0 + x + Ox;

  glRasterPos2f(x, y);
  for(int i=0;i<len;i++){
  	glutBitmapCharacter(font, s[i]);
  }
}

void drawRectangle(float x, float y, float width, float height, float Ox, float Oy, GLenum type) {
	if (type == 0) type = GL_POLYGON;

	//(x, y) —> Top-left corner
	y = -1.0 + y + Oy;
	x = -1.0 + x + Ox;

	// Start drawing
	glBegin(type);

	// Set the rectangle vertices
	glVertex2f(x, y - height);
	glVertex2f(x + width, y - height);
	glVertex2f(x + width, y);
	glVertex2f(x, y);

	// End drawing
	glEnd();
}
void setFigureColor(int c) {
	glColor3f(colors[c][0], colors[c][1], colors[c][2]);
}

void draw(void) {

    // Start drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Invert Y axis
    gluOrtho2D(-1.0, 1.0, 1.0, -1.0);

    float Ox = 1.0 - N * S * 0.5; // Offset for the game field
    float Oy = 1.0 - M * S * 0.5;
    // Draw the current figure
    for (int i = 0; i < 4; i++) {
        // Draw all blocks in the figure, regardless of their position
        setFigureColor(shape);
        drawRectangle(a[i].x * S, a[i].y * S, S, S, Ox, Oy, 0);
    }

    // Draw the next figure outside the game field
    if (!gameOver) {
        glColor3f(1.0, 1.0, 1.0);
        drawText(0, 0, (char *)"Next:", Ox + N * S * 0.6, Oy - 5 * S, NULL);

        // Offset for the "Next figure" display
        float nextFigureOffsetX = Ox +  S * 7;
        float nextFigureOffsetY = Oy - 6 * S;

        for (int i = 0; i < 4; i++) {
            setFigureColor(shapeNext);
            drawRectangle(c[i].x * S, c[i].y * S, S, S, nextFigureOffsetX, nextFigureOffsetY, 0);
        }
    }

    if(!gameOver){
    	glColor3f(1.0, 1.0, 1.0);
		drawText(-0.56, 0.2, "Score: ", Ox + N * S * 0.6, Oy - 9 * S, NULL);
		glColor3f(1.0, 1.0, 1.0);
		drawScore(-0.1, 0.1, score, Ox + 8.2 * S, Oy - 7 * S, NULL);
	}

    // Draw the field squares
    for (int i = 0; i < M; i++) { 
        for (int j = 0; j < N; j++) {
                if (field[i][j] != 0) {
                    setFigureColor(field[i][j] - 1);
                    drawRectangle(j * S, i * S, S, S, Ox, Oy, 0);
                }
            // Draw the grid
            glColor3f(0.4, 0.4, 0.4);
            drawRectangle(j * S, i * S, S, S, Ox, Oy, GL_LINE_LOOP);
        }
    }

    // Game over message
    if (gameOver) {
        glColor3f(1.0, 0.3, 0.0);
        drawText(0, 0, (char *)"Game over!", Ox + 8*S, Oy - 3 * S, NULL);
    }

    glutSwapBuffers();
}


void clearColumn(void) {
    // Check for full columns and clear them
    
    for (int j = 0; j < N; j++) {
        int fullColumn = 1;

        // Check if the entire column is filled
        for (int i = 0; i < M; i++) {
            if (field[i][j] == 0) {
                fullColumn = 0; // If any cell is empty, column is not full
                break;
            }
        }

        if (fullColumn) {
        	score += 10;
            // Clear the full column and shift other columns to the left
            for (int i = 0; i < M; i++) {
                // Shift all columns to the right of the filled column to the left
                for (int k = j; k < N - 1; k++) {
                    field[i][k] = field[i][k + 1];
                }
                // Set the last column in the row to 0 (empty)
                field[i][N - 1] = 0;
            }
        }
    }

}

void timerFunc(int value) {
    // Move the piece left by one unit
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x -= 1;  // Move left
    }

    // Check for collision with existing blocks or the left wall
    if (!check()){
        // Lock the piece in place (add it to the field)
        for (int i = 0; i < 4; i++){
            field[b[i].y][b[i].x] = shape+1;
        }
        clearColumn();
        if (!gameOver) {
            spawnFigure();
        }
    }

    // Redraw the screen
    glutPostRedisplay();

    // Reset the timer
    if (!gameOver){
        glutTimerFunc(delay, timerFunc, 0);
    }
}

void drop() {
	while(1)
    {
        for (int i = 0; i < 4; i++) {
		b[i] = a[i];
		a[i].x += (-1);
        }

        // Reset the figure if it's out of bounds
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
            break;
        }
    }	
}

void rotate(void) {
	// Rotation around a[1]
	point p = a[1];

	for (int i = 0; i < 4; i++) {
		int x = a[i].y - p.y;
		int y = a[i].x - p.x;

		a[i].x = p.x - x;
		a[i].y = p.y + y;
	}

	// Reset the figure if it's out of bounds
	if (!check()) {
		for (int i = 0; i < 4; i++) {
			a[i] = b[i];
		}
	}
}


void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC
            exit(0);
            break;
        case 'u':  // Move up
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y -= 1;
            }
            // Ensure the figure doesn't move out of bounds or collide
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i];
                }
            }
            break;
        case 'd':  // Move down
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }
            // Ensure the figure doesn't move out of bounds or collide
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i];
                }
            }
            break;
        case 'a':  
            drop();
            break;
        case 32:  // Rotate
            rotate();
            break;
    }
    glutPostRedisplay();
}



void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	srand(time(NULL));

	generateFigure();
	spawnFigure();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Tetris");

	init();

	glutDisplayFunc(draw);
	glutTimerFunc(delay, timerFunc, 0);
	glutKeyboardFunc(keyboardFunc);
	glutMainLoop();

	return 0;
}