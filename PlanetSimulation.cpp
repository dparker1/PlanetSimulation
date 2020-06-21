// PlanetSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "PlanetSystem.h"

#define PI 3.1415926535897932384626433832795
#define TWOPI 6.283185307179586476925286766559
#define CIRCLE_VERTICES 40
#define BASE_CIRCLE_RADIUS 0.001

int window;
int windowWidth = 900, windowHeight = 900;
double halfWidth = windowWidth / 2.0, halfHeight = windowHeight / 2.0;
bool leftClick, rightClick;
double trueX, trueY, trueXEnd, trueYEnd;
Planet newPlanet;
std::chrono::steady_clock::time_point oldTime, newTime;

PlanetSystem s;

void drawPlanet(Planet* p)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(p->position.x, p->position.y);
	double radius = BASE_CIRCLE_RADIUS * p->size;
	for (int i = 0; i <= CIRCLE_VERTICES; i++)
	{
		glVertex2d(radius * cos(i * TWOPI / CIRCLE_VERTICES) + p->position.x,
			radius * sin(i * TWOPI / CIRCLE_VERTICES) + p->position.y);
	}
	glEnd();
}

void drawPlanets()
{
	glColor3d(1, 1, 1);
	for (std::vector<Planet>::iterator it = s.planets.begin(); it != s.planets.end(); ++it)
	{
		drawPlanet(&(*it));
	}
}

void drawGhost()
{
	glColor4d(1, 1, 1, 0.5);
	drawPlanet(&newPlanet);
}

void drawArrow()
{
	if (leftClick)
	{
		glColor3d(1, 0, 0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2d(trueX, trueY);
		glVertex2d(trueXEnd, trueYEnd);
		glEnd();
	}
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
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightClick = true;
	}

	if (leftClick && state == GLUT_UP)
	{
		leftClick = false;
		newPlanet.velocity.x = (trueXEnd - trueX) / 50;
		newPlanet.velocity.y = (trueYEnd - trueY) / 50;
		s.addPlanet(newPlanet);
		std::cout << "Size: " << newPlanet.size << ", Mass: " << newPlanet.mass << std::endl;
	}

	if (rightClick && state == GLUT_UP)
	{
		rightClick = false;
		IntDouble iDist = s.closestPlanet({ trueX, trueY });
		
		if (iDist.i != -1 && iDist.d < 2 * pow(s.planets[iDist.i].size * BASE_CIRCLE_RADIUS, 2))
		{
			s.removePlanet(iDist.i);
		}
	}
}

void passiveMouseCallback(int x, int y)
{
	newPlanet.position.x = (x / halfWidth) - 1;
	newPlanet.position.y = 1 - (y / halfHeight);
	trueX = newPlanet.position.x;
	trueY = newPlanet.position.y;
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
		newPlanet.mass = pow(newPlanet.size, 3) / 100;
	}
	else
	{
		newPlanet.size--;
		newPlanet.mass = pow(newPlanet.size, 3) / 100;
	}
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if (key == 'f')
	{
		newPlanet.fixed = !newPlanet.fixed;
	}

	if (key == 27) // esc
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
	s.addPlanet(20, 80, { 0, 0 }, true);

	glutKeyboardFunc(keyboardCallback);
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
