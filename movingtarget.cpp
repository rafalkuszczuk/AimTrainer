/**
 * @file MovingTarget.cpp
 * @author Rafał Kuszczuk
 * @brief Zaawansowana interakcja, "raw input", matematyka detekcji kolizji (hitboxy) i wektory ruchu.
 */
#include "MovingTarget.h"

using namespace std;

MovingTarget::MovingTarget(float startX, float startY, float r, float vX, float vY, float screenW, float screenH)
    : Target(startX, startY, r), maxX(screenW), maxY(screenH) {

    // predkosc w poziomie i pionie
    velocityX = vX;
    velocityY = vY;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);

    // ruchoma tarcza jest czerwona
    shape.setFillColor(sf::Color::Red);
}

void MovingTarget::setColor(sf::Color color) {
    shape.setFillColor(color);
}

void MovingTarget::update(float deltaTime) {
    // przesuwamy tarcze dodajac predkosc
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // jak uderzy w lewa sciane to odbijamy w prawo
    if (x - radius <= 0) {
        x = radius;
        velocityX = -velocityX;
        // jak uderzy w prawa sciane to odbijamy w lewo
    } else if (x + radius >= maxX) {
        x = maxX - radius;
        velocityX = -velocityX;
    }

    // to samo dla sufitu i podlogi
    if (y - radius <= 0) {
        y = radius;
        velocityY = -velocityY;
    } else if (y + radius >= maxY) {
        y = maxY - radius;
        velocityY = -velocityY;
    }

    // zapisujemy gdzie teraz jest
    shape.setPosition(x, y);

    // identyczne znikanie jak przy statycznej tarczy
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
