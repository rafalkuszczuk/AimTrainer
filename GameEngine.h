#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <random>
#include "Target.h"
#include "InputManager.h"
#include "StatsManager.h"

using namespace std;

class GameEngine {
private:
    std::string playerName;

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
    bool hasFont;

    float trackingTimeRequired;
    float currentTrackTime;
    float timeLimit;
    float elapsedTime;
    int targetSizeOption;
    bool useDDA; // <--- NOWA ZMIENNA

    sf::Texture crosshairTex;
    bool hasCrosshairTex;

    sf::SoundBuffer shootBuf, hitBuf, deadBuf;
    sf::Sound shootSound, hitSound, deadSound;
    bool hasShootSound, hasHitSound, hasDeadSound;

    float ddaMultiplier = 1.0f;

    bool isGameOver;
    bool gameOverSoundPlayed;
    sf::Texture najmanTex;
    sf::Sprite najmanSprite;
    sf::SoundBuffer najmanBuf;
    sf::Sound najmanSound;
    bool hasNajmanTex, hasNajmanSound;

    void processEvents();
    void update(float deltaTime);
    void render();

    void spawnRandomTarget();
    void drawUI();
    void updateDDA();

public:
    // Zmieniony konstruktor z enableDDA
    GameEngine(std::string name, int mode, float trackReq = 0.0f, float limit = 0.0f, int sizeOpt = 4, bool enableDDA = true);
    void run();
};
