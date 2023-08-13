#include <math.h>		
#include <stdio.h>
#include <stdlib.h>		
#include "Glut.h"
#include <iostream>   
#include <string>     




int width = 640;     // Sirina prozora
int height = 480;    // Visina prozora
int paddleWidth = 10;  // Sirina reketa
int paddleHeight = 60; // Visina reketa
float paddleSpeed = 10.0f;  // Brzina pomeranja reketa
float paddle_redY = 0.0f;  // Y-koordinata prvog reketa
float paddle_blueY = 0.0f;  // Y-koordinata drugog reketa
float ballX = 0.0f;    // X-koordinata lopte
float ballY = 0.0f;    // Y-koordinata lopte
float ballSpeedX = 3.0f;  // Horizontalna brzina lopte
float ballSpeedY = 3.0f;  // Vertikalna brzina lopte
int score_red = 0;  // Poeni prvog igraca
int score_blue = 0;  // Poeni drugog igraca
bool gameEnd = false;


void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-width / 2, width / 2, -height / 2, height / 2);
}

// Funkcija za crtanje lopte
void drawCircle(float cx, float cy, float r, int numSegments) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy);  // Postavljanje centra
	for (int i = 0; i <= numSegments; ++i) {
		float z = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float x = r * cosf(z); 
		float y = r * sinf(z); 
		glVertex2f(x + cx, y + cy); 
	}
	glEnd();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (!gameEnd){
		// Crtanje reketa
		glBegin(GL_QUADS);
		// Crveni reket
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-width / 2 + paddleWidth, paddle_redY + paddleHeight / 2);
		glVertex2f(-width / 2, paddle_redY + paddleHeight / 2);
		glVertex2f(-width / 2, paddle_redY - paddleHeight / 2);
		glVertex2f(-width / 2 + paddleWidth, paddle_redY - paddleHeight / 2);
		// Plavi reket
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(width / 2 - paddleWidth, paddle_blueY + paddleHeight / 2);
		glVertex2f(width / 2, paddle_blueY + paddleHeight / 2);
		glVertex2f(width / 2, paddle_blueY - paddleHeight / 2);
		glVertex2f(width / 2 - paddleWidth, paddle_blueY - paddleHeight / 2);
		glEnd();

		// Crtanje lopte 
		
		glColor3f(1.0f, 1.0f, 0.0f);
		
		drawCircle(ballX, ballY, 5.0f, 20);

		// Ispisivanje rezultata
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(-50.0f, height / 2 - 30.0f);
		std::string scoreText = "Red: " + std::to_string(score_red) + "    Blue: " + std::to_string(score_blue);
		for (const char& c : scoreText) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}

	else{
		// Ispisivanje pobednika
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(-50.0f, 0.0f);
		std::string winnerText = (score_red == 11 ? "Red wins!" : "Blue wins!");
		for (const char& c : winnerText) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}

	glutSwapBuffers();
}


void update(int value) {
	if (!gameEnd) {
		// Azuriranje pozicije lopte
		ballX += ballSpeedX;
		ballY += ballSpeedY;

		// Sudaranje sa reketima
		if (ballX >= width / 2 - paddleWidth && ballY <= paddle_blueY + paddleHeight / 2 && ballY >= paddle_blueY - paddleHeight / 2){

			ballSpeedX = -ballSpeedX;
			ballSpeedX *= 1.15f;
		}

		else if (ballX <= -width / 2 + paddleWidth && ballY <= paddle_redY + paddleHeight / 2 && ballY >= paddle_redY - paddleHeight / 2){

			ballSpeedX = -ballSpeedX;
			ballSpeedX *= 1.15f;
		}

		// Sudaranje sa zidovima
		if (ballY >= height / 2 || ballY <= -height / 2)

			ballSpeedY = -ballSpeedY;

		// Azuriranje rezultata
		if (ballX >= width / 2) {
			score_red++;

			ballX = 0.0f;
			ballY = 0.0f;

			ballSpeedX = -3.0f;
			ballSpeedY = 3.0f;
		}
		else if (ballX <= -width / 2) {
			score_blue++;

			ballX = 0.0f;
			ballY = 0.0f;

			ballSpeedX = 3.0f;
			ballSpeedY = 3.0f;
		}

		if (score_red >= 11 || score_blue >= 11) {
			gameEnd = true;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(16, update, 0);
}



// Pomeranje plavog reketa
void specialKeyboard(int key, int x, int y) {
	// Pomeranje plavog reketa na gore
	if (key == GLUT_KEY_UP)
		paddle_blueY += paddleSpeed;
	// Pomeranje plavog reketa na dole
	else if (key == GLUT_KEY_DOWN)
		paddle_blueY -= paddleSpeed;
}

// Pomeranje crvenog reketa misem
void mouse(int x, int y) {
	paddle_redY = height / 2 - y;
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pong");
	init();
	glutDisplayFunc(draw);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(0, update, 0);
	glutMainLoop();
	return 0;
}
