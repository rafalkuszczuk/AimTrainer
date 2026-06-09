#include "StaticTarget.h"

using namespace std;

StaticTarget::StaticTarget(float startX, float startY, float r) : Target(startX, startY, r) {
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue); // Statyczny cel będzie niebieski
}

void StaticTarget::update(float deltaTime) {
    // Cel statyczny stoi w miejscu, więc update jest puste
}

void StaticTarget::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
