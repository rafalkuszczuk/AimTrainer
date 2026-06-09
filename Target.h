#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Target {
protected:
    float x, y, radius;
    bool shrinking = false; // Flaga wyzwalająca animację
    bool dead = false;      // Flaga oznaczająca koniec animacji (do usunięcia)

public:
    Target(float startX, float startY, float r) : x(startX), y(startY), radius(r) {}

    virtual ~Target() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

    // Metody do obsługi animacji i usuwania
    void startShrinking() { shrinking = true; }
    bool isDead() const { return dead; }
};
