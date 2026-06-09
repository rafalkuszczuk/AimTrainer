#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class InputManager {
private:
    float sensitivity;
    float virtualX;
    float virtualY;

public:
    InputManager(float sens, float startX, float startY);

    void update(sf::RenderWindow& window);

    float getX() const;
    float getY() const;

    bool checkCollision(float targetX, float targetY, float targetRadius) const;
};
