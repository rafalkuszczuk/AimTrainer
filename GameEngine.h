/**
 * @file GameEngine.h
 * @author Filip Krzyżkowiak
 * @brief Architektura silnika, inicjalizacja okna, główna pętla gry, zarządzanie pamięcią oraz polimorfizm obiektów.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Target.h"
#include "InputManager.h"
#include "StatsManager.h"
#include "UIManager.h"

// stany w jakich moze byc nasza gra
enum GameState { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER };

class GameEngine {
private:
    GameState currentState; // gdzie aktualnie jestesmy
    sf::RenderWindow window; // glowne okno z gra
    UIManager ui; // zarzadca menu i napisow

    std::vector<std::unique_ptr<Target>> targets; // worek na nasze kółka
    InputManager inputManager; // obsluga celownika
    StatsManager stats; // nasz ksiegowy od wynikow
    std::mt19937 rng; // maszyna losujaca

    sf::CircleShape crosshair; // kropka celownika
    sf::Texture crosshairTex;
    bool hasCrosshairTex;

    // pliki dzwiekowe do ladowania
    sf::SoundBuffer shootBuf, hitBuf, deadBuf;
    sf::Sound shootSound, hitSound, deadSound;
    bool hasShootSound, hasHitSound, hasDeadSound;

    // rozne liczniki potrzebne do gry
    int lives;
    float spawnTimer;
    float trackingTimeRequired;
    float currentTrackTime;
    float timeLimit;
    float elapsedTime;

    // liczniki tylko do trybu na refleks
    int reflexState;
    float reflexWaitTime;
    float reflexTimer;
    float lastReactionTime;

    float mouseSensitivity;
    float ddaMultiplier; // mnoznik trudnosci (im mniejszy tym trudniej)

    bool gameOverSoundPlayed;
    bool isResultSaved;

    // wewnetrzne funkcje silnika
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnRandomTarget();
    void updateDDA();
    void resetGameSession();

public:
    GameEngine();
    void run(); // to odpala cala zabawe
};
