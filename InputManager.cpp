#include "InputManager.h"

using namespace std;

InputManager::InputManager(float sens, float startX, float startY) {
    sensitivity = sens;
    virtualX = startX;
    virtualY = startY;
}

void InputManager::update(sf::RenderWindow& window) {
    // Upewniamy się, że gra jest aktywnym oknem, by nie "kraść" myszy systemowi
    if (!window.hasFocus()) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();

    // Wyznaczamy środek ekranu
    sf::Vector2i center(windowSize.x / 2, windowSize.y / 2);

    // Obliczamy deltę przesunięcia od środka okna
    float deltaX = static_cast<float>(mousePos.x - center.x);
    float deltaY = static_cast<float>(mousePos.y - center.y);

    // Aplikujemy czułość niczym w CS2 (Sensitivity)
    virtualX += deltaX * sensitivity;
    virtualY += deltaY * sensitivity;

    // Opcjonalne blokowanie wirtualnego celownika, by nie wyjechał poza okno
    if (virtualX < 0) virtualX = 0;
    if (virtualX > windowSize.x) virtualX = windowSize.x;
    if (virtualY < 0) virtualY = 0;
    if (virtualY > windowSize.y) virtualY = windowSize.y;

    // "Zablokowanie" systemowego kursora z powrotem na środku (imitacja Raw Input)
    sf::Mouse::setPosition(center, window);
}

float InputManager::getX() const { return virtualX; }
float InputManager::getY() const { return virtualY; }

bool InputManager::checkCollision(float targetX, float targetY, float targetRadius) const {
    float dx = virtualX - targetX;
    float dy = virtualY - targetY;

    // Optymalizacja: zamiast używać kosztownego pierwiastkowania (sqrt) do obliczenia odległości,
    // porównujemy kwadrat odległości z kwadratem promienia.
    float distanceSquared = (dx * dx) + (dy * dy);
    float radiusSquared = targetRadius * targetRadius;

    return distanceSquared <= radiusSquared;
}
