#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm> // Do niszczenia tarcz
#include "Target.h"
#include "InputManager.h" // Podpięcie Twojego modułu

using namespace std;

class GameEngine {
private:
    sf::RenderWindow window;
    vector<unique_ptr<Target>> targets;

    InputManager inputManager; // Twój obiekt zarządzający myszką
    sf::CircleShape crosshair; // Nasz wizualny celownik

    void processEvents();
    void update(float deltaTime);
    void render();

public:
    GameEngine();
    void run();
    void spawnTestTargets();
};
