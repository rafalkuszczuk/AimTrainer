#pragma once
#include "Target.h"
#include <SFML/Graphics.hpp>

using namespace std;

class MovingTarget : public Target {
private:
    float velocityX;
    float velocityY;
    float maxX; // Granica X ekranu
    float maxY; // Granica Y ekranu
    sf::CircleShape shape;

public:
    MovingTarget(float startX, float startY, float r, float vX, float vY, float screenW, float screenH);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void setColor(sf::Color color) override;
};
