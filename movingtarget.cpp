#include "MovingTarget.h"

using namespace std;

MovingTarget::MovingTarget(float startX, float startY, float r, float vX, float vY, float screenW, float screenH)
    : Target(startX, startY, r), maxX(screenW), maxY(screenH) {

    velocityX = vX;
    velocityY = vY;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
}

void MovingTarget::setColor(sf::Color color) {
    shape.setFillColor(color);
}

void MovingTarget::update(float deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // FIZYKA: Odbijanie od krawędzi okna!
    if (x - radius <= 0) {
        x = radius;
        velocityX = -velocityX; // Odwrócenie wektora X
    } else if (x + radius >= maxX) {
        x = maxX - radius;
        velocityX = -velocityX;
    }

    if (y - radius <= 0) {
        y = radius;
        velocityY = -velocityY; // Odwrócenie wektora Y
    } else if (y + radius >= maxY) {
        y = maxY - radius;
        velocityY = -velocityY;
    }

    shape.setPosition(x, y);

    if (shrinking) {
        radius -= 250.0f * deltaTime;
        if (radius <= 0) {
            radius = 0;
            dead = true;
        } else {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius);
        }
    }
}

void MovingTarget::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
