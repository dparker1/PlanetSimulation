#include "PlanetSystem.h"
#include <vector>
#include <math.h>

double distance(Vec2 p1, Vec2 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

PlanetSystem::PlanetSystem()
{
	this->n = 0;
}

PlanetSystem::~PlanetSystem()
{

}

void PlanetSystem::addPlanet(Planet p)
{
	this->planets.push_back(p);
	n++;
}

void PlanetSystem::addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force, bool fixed)
{
	Planet p = { size, mass, position, velocity, force, fixed };
	this->planets.push_back(p);
	n++;
}

void PlanetSystem::addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force)
{
	addPlanet(size, mass, position, velocity, force, false);
}

void PlanetSystem::addPlanet(int size, double mass, Vec2 position, Vec2 velocity, bool fixed)
{
	addPlanet(size, mass, position, velocity, {0, 0}, fixed);
}

void PlanetSystem::addPlanet(int size, double mass, Vec2 position, bool fixed)
{
	addPlanet(size, mass, position, {0, 0}, { 0, 0 }, fixed);
}

void PlanetSystem::addPlanet(int size, double mass, Vec2 position, Vec2 velocity)
{
	addPlanet(size, mass, position, velocity, { 0, 0 }, false);
}

void PlanetSystem::calculate(double step)
{
	double d, inter;
	for (int i = 0; i < this->n; i++)
	{
		Planet* p = &(this->planets[i]);
		p->position.x += step * p->velocity.x;
		p->position.y += step * p->velocity.y;
		p->velocity.x += step * p->force.x;
		p->velocity.y += step * p->force.y;
		p->force.x = 0;
		p->force.y = 0;
		for (int j = 0; j < this->n; j++)
		{
			if (j == i || p->fixed)
			{
				continue;
			}
			Planet* x = &(this->planets[j]);
			d = distance(p->position, x->position);
			inter = (G * x->mass) / (d * d * d);
			p->force.x -= inter * (p->position.x - x->position.x);
			p->force.y -= inter * (p->position.y - x->position.y);
		}
	}
}

int PlanetSystem::nPlanets()
{
	return this->n;
}