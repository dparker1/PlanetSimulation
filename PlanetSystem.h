#pragma once
#include <vector>

#define G 0.000000001

struct Vec2 {
    double x{ 0 };
    double y{ 0 };
};

struct IntDouble
{
    int i{ 0 };
    double d{ 0 };
};

struct Planet {
    int size{ 10 };
    double mass{ 10 };
    Vec2 position;
    Vec2 velocity;
    Vec2 force;
    bool fixed{ false };
};

class PlanetSystem
{
public:
    PlanetSystem();
    ~PlanetSystem();
    void addPlanet(Planet p);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity);
    void removePlanet(int planetIndex);
    IntDouble closestPlanet(Vec2 pos);
    void calculate(double step);
    int nPlanets();

    std::vector<Planet> planets;

private:
    int n;
};