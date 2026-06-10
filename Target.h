#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Target {
protected:
    float x, y, radius;
    bool shrinking = false;
    bool dead = false;

    // Nowe zmienne dla trybu Survival
    float timeAlive = 0.0f;
    bool isPenaltyTarget = false;

public:
    Target(float startX, float startY, float r) : x(startX), y(startY), radius(r) {}
    virtual ~Target() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setColor(sf::Color color) = 0; // Czysto wirtualna zmiana koloru

    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

    void startShrinking() { shrinking = true; }
    bool isDead() const { return dead; }

    // Gettery i Settery dla nowej logiki
    void addTimeAlive(float dt) { timeAlive += dt; }
    float getTimeAlive() const { return timeAlive; }
    void setPenalty(bool penalty) { isPenaltyTarget = penalty; }
    bool isPenalty() const { return isPenaltyTarget; }
};
