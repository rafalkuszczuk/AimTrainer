#include "StaticTarget.h"

using namespace std;

StaticTarget::StaticTarget(float startX, float startY, float r) : Target(startX, startY, r) {
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Blue); // Statyczny cel będzie niebieski
}

void StaticTarget::update(float deltaTime) {
    if (shrinking) {
        // Zmniejszamy promień o 250 pikseli na sekundę
        radius -= 250.0f * deltaTime;

        if (radius <= 0) {
            radius = 0;
            dead = true; // Koniec animacji, można usunąć z wektora
        } else {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius); // Zmiana origin by kurczyła się do środka
        }
    }
}

void StaticTarget::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
void StaticTarget::setColor(sf::Color color) {
    shape.setFillColor(color);
}

