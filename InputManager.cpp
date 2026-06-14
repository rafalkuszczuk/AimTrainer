#include "InputManager.h"

using namespace std;

InputManager::InputManager(float sens, float startX, float startY) {
    sensitivity = sens;
    // gdzie zaczyna sie nasz celownik
    virtualX = startX;
    virtualY = startY;
}

void InputManager::update(sf::RenderWindow& window) {
    // jak zminimalizowalismy gre, to nie czytamy myszki
    if (!window.hasFocus()) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();

    // szukamy idealnego srodka ekranu
    sf::Vector2i center(windowSize.x / 2, windowSize.y / 2);

    // patrzymy o ile pikseli ruszylismy myszka od srodka
    float deltaX = static_cast<float>(mousePos.x - center.x);
    float deltaY = static_cast<float>(mousePos.y - center.y);

    // przesuwamy nasz celownik o ten ruch razy czulosc
    virtualX += deltaX * sensitivity;
    virtualY += deltaY * sensitivity;

    // nie pozwalamy celownikowi uciec poza ekran
    if (virtualX < 0) virtualX = 0;
    if (virtualX > windowSize.x) virtualX = windowSize.x;
    if (virtualY < 0) virtualY = 0;
    if (virtualY > windowSize.y) virtualY = windowSize.y;

    // wrzucamy prawdziwa myszke na srodek, zeby jej nie zabraklo na podkladce
    sf::Mouse::setPosition(center, window);
}

float InputManager::getX() const { return virtualX; }
float InputManager::getY() const { return virtualY; }

bool InputManager::checkCollision(float targetX, float targetY, float targetRadius) const {
    // liczymy roznice odleglosci
    float dx = virtualX - targetX;
    float dy = virtualY - targetY;

    // uzywamy twierdzenia pitagorasa zeby uniknac ciezkich pierwiastkow
    float distanceSquared = (dx * dx) + (dy * dy);
    float radiusSquared = targetRadius * targetRadius;

    // jak odleglosc jest mniejsza niz wielkosc kółka to mamy trafienie
    return distanceSquared <= radiusSquared;
}
