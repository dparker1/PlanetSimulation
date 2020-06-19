// PlanetSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "PlanetSystem.h"

int window;
int windowWidth = 1600, windowHeight = 900;
double halfWidth = windowWidth / 2.0, halfHeight = windowHeight / 2.0;
bool leftClick, rightClick;
double trueX, trueY, trueXEnd, trueYEnd;
Planet newPlanet;
std::chrono::steady_clock::time_point oldTime, newTime;

PlanetSystem s;

void drawPlanets()
{
	glColor3d(1, 1, 1);
	for (std::vector<Planet>::iterator it = s.planets.begin(); it != s.planets.end(); ++it)
	{
		glPointSize(it->size);
		glBegin(GL_POINTS);
		glVertex2d(it->position.x, it->position.y);
		glEnd();
	}
}

void drawGhost()
{
	glColor4d(1, 1, 1, 0.5);
	glPointSize(newPlanet.size);
	glBegin(GL_POINTS);
	glVertex2d(trueX, trueY);
	glEnd();
}

void drawArrow()
{
	glColor3d(1, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2d(trueX, trueY);
	glVertex2d(trueXEnd, trueYEnd);
	glEnd();
}

void render()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawPlanets();
	drawGhost();
	drawArrow();

	newTime = std::chrono::steady_clock::now();
	long long diff = std::chrono::duration_cast<std::chrono::microseconds>(newTime - oldTime).count();
	if (diff > 0)
	{
		oldTime = newTime;
		double step = 1.0 / (diff) * 1000;
		s.calculate(step);
	}
	oldTime = newTime;

	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	halfWidth = w / 2.0;
	halfHeight = h / 2.0;
}

void mouseCallback(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		leftClick = true;
		newPlanet.position.x = (x / halfWidth) - 1;
		newPlanet.position.y = 1 - (y / halfHeight);
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightClick = true;
	}

	if (leftClick && state == GLUT_UP)
	{
		newPlanet.velocity.x = (trueXEnd - trueX) / 50;
		newPlanet.velocity.y = (trueYEnd - trueY) / 50;
		s.addPlanet(newPlanet);
		std::cout << newPlanet.size << ": " << newPlanet.mass << std::endl;
	}
}

void passiveMouseCallback(int x, int y)
{
	trueX = (x / halfWidth) - 1;
	trueY = 1 - (y / halfHeight);
	trueXEnd = trueX;
	trueYEnd = trueY;
}

void activeMouseCallback(int x, int y)
{
	trueXEnd = (x / halfWidth) - 1;
	trueYEnd = 1 - (y / halfHeight);
}

void mouseWheelCallback(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		newPlanet.size++;
		newPlanet.mass++;
	}
	else
	{
		newPlanet.size--;
		newPlanet.mass--;
	}
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char** argv)
{
	oldTime = std::chrono::steady_clock::now();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	window = glutCreateWindow("Planet Simulation");
	s = PlanetSystem();
	s.addPlanet(10, 10, { 0.15, 0 }, { 0, 0.00025 });
	s.addPlanet(20, 20, { 0, 0 }, true);

	glutMouseFunc(mouseCallback);
	glutMotionFunc(activeMouseCallback);
	glutPassiveMotionFunc(passiveMouseCallback);
	glutMouseWheelFunc(mouseWheelCallback);
	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutIdleFunc(render);
	glutMainLoop();

	exit(EXIT_SUCCESS);
	return 0;
}