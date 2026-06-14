/**
 * @file MovingTarget.h
 * @author Rafał Kuszczuk
 * @brief Zaawansowana interakcja, "raw input", matematyka detekcji kolizji (hitboxy) i wektory ruchu.
 */
#pragma once
#include "Target.h"
#include <SFML/Graphics.hpp>

using namespace std;

// Dziedziczy po 'Target', ale dorzuca do równania PRĘDKOŚĆ i odbijanie od ścian.
class MovingTarget : public Target {
private:
    float velocityX; // Prędkość po osi X (ruch w lewo/prawo)
    float velocityY; // Prędkość po osi Y (ruch w górę/dół)
    float maxX;      // Granica ekranu w prawo (szerokość okna)
    float maxY;      // Granica ekranu w dół (wysokość okna)
    sf::CircleShape shape; // Fizyczny kształt koła od SFML

public:
    MovingTarget(float startX, float startY, float r, float vX, float vY, float screenW, float screenH);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void setColor(sf::Color color) override;
};
