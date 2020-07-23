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
    double size{ 10 };
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
    PlanetSystem(double collisionThreshold);
    ~PlanetSystem();
    void addPlanet(Planet p);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, Vec2 force);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, bool fixed);
    void addPlanet(int size, double mass, Vec2 position, Vec2 velocity);
    void removePlanet(int planetIndex);
    void cullPlanets();
    IntDouble closestPlanet(Vec2 pos);
    void calculate(double step);
    int nPlanets();

    std::vector<Planet> planets;
    double collisionThreshold;

private:
    int n;
};