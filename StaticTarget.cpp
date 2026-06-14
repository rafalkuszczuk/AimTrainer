/**
 * @file StaticTarget.cpp
 * @author Rafał Kuszczuk
 * @brief Zaawansowana interakcja, implementacja "raw input", matematyka detekcji kolizji oraz wektory ruchu.
 */
#include "StaticTarget.h"

using namespace std;

StaticTarget::StaticTarget(float startX, float startY, float r) : Target(startX, startY, r) {
    shape.setRadius(radius);

    // ustawiamy srodek chwytu w centrum kółka, zeby sie kurczylo do srodka a nie do rogu
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);

    // zwykła tarcza jest na start niebieska
    shape.setFillColor(sf::Color::Blue);
}

void StaticTarget::update(float deltaTime) {
    // jak ktos w to trafil, to zaczynamy kurczenie
    if (shrinking) {
        // zmniejszamy promien, mnozymy przez czas zeby na kazdym kompie dzialalo tak samo
        radius -= 250.0f * deltaTime;

        // jak zniknie calkiem to oznaczamy jako martwe
        if (radius <= 0) {
            radius = 0;
            dead = true;
        } else {
            // aktualizujemy wyglad
            shape.setRadius(radius);
            shape.setOrigin(radius, radius);
        }
    }
}

void StaticTarget::draw(sf::RenderWindow& window) {
    window.draw(shape); // rysuje kółko na ekranie
}

void StaticTarget::setColor(sf::Color color) {
    shape.setFillColor(color); // zmienia kolor jak trzeba
}
