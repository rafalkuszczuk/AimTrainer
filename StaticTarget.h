#pragma once
#include "Target.h"

using namespace std;

class StaticTarget : public Target {
private:
    sf::CircleShape shape;

public:
    StaticTarget(float startX, float startY, float r);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};
