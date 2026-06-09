#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Target {
protected:
    float x, y, radius;

public:
    Target(float startX, float startY, float r) : x(startX), y(startY), radius(r) {}

    virtual ~Target() = default; // Wirtualny destruktor to podstawa polimorfizmu

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};
