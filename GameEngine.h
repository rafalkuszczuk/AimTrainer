#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "Target.h"
#include "InputManager.h"
#include "StatsManager.h"

using namespace std;

class GameEngine {
private:
    sf::RenderWindow window;
    vector<unique_ptr<Target>> targets;
    InputManager inputManager;
    sf::CircleShape crosshair;

    StatsManager stats;
    std::mt19937 rng;
    int gameMode;
    int lives;
    float spawnTimer;

    sf::Font font;
    sf::Text uiText;

    float trackingTimeRequired;
    float currentTrackTime;
    float timeLimit;
    float elapsedTime;
    int targetSizeOption; // Nowa zmienna dla rozmiaru tarczy

    void processEvents();
    void update(float deltaTime);
    void render();

    void spawnRandomTarget();
    void drawUI();

public:
    GameEngine(int mode, float trackReq = 0.0f, float limit = 0.0f, int sizeOpt = 4);
    void run();
};
