#include "MovingTarget.h"

using namespace std;

MovingTarget::MovingTarget(float startX, float startY, float r, float vX, float vY)
    : Target(startX, startY, r) {

    velocityX = vX;
    velocityY = vY;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
}

void MovingTarget::update(float deltaTime) {
    // Ruch tarczy z Twojej fizyki
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
    shape.setPosition(x, y);

    // Animacja niszczenia
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
