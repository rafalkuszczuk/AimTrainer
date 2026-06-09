#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Target.h"

using namespace std;

class GameEngine {
private:
    sf::RenderWindow window;
    vector<unique_ptr<Target>> targets; // Kontener polimorficzny (Wymóg 5)

    void processEvents();
    void update(float deltaTime);
    void render();

public:
    GameEngine();
    void run();
    void spawnTestTargets();
};
