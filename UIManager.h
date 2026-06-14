/**
 * @file UIManager.h
 * @author Sebastian Mielcarek
 * @brief Logika UI, implementacja algorytmu DDA, zapis analityki do plików (I/O) oraz wczytywanie zasobów.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "StatsManager.h"

// ogarnia całe rysowanie menu i wynikow, zeby nie robic bałaganu w silniku
class UIManager {
public:
    // ustawienia pobierane z menu przez gracza
    std::string playerName;
    std::string sensString;
    std::string trackString;
    int activeField;
    int gameMode;
    int targetSizeOption;
    float selectedTimeLimit;
    bool useDDA;
    sf::Color currentBgColor;
    sf::Color currentChColor;

    // znaki dla silnika zeby cos zrobil
    bool startGameRequested;
    bool backToMenuRequested;

    UIManager();
    void loadResources(float width, float height);

    // ogarnianie logiki klikania i wpisywania z klawiatury
    void handleTextInput(sf::Uint32 unicode);
    void handleMenuClick(sf::Vector2f mousePos);
    void handleGameOverClick(sf::Vector2f mousePos, StatsManager& stats, bool& isResultSaved);

    // rysowanie
    void drawMenu(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, float elapsedTime, StatsManager& stats, int lives);
    void drawHUD(sf::RenderWindow& window, float elapsedTime, float timeLimit, int lives, float currentTrackTime, float trackingTimeRequired, float ddaMultiplier, StatsManager& stats);
    void drawReflexInstruct(sf::RenderWindow& window, int reflexState, float lastReactionTime);

    void playNajmanSound();

    // tu dodalismy nasz fix dla guzikow
    void resetSaveUI();

private:
    sf::Font font;
    bool hasFont;

    // masa klockow do wyswietlania napisow na ekranie
    sf::Text uiText, menuTitle, inputLabel, sensLabel, trackLabel;
    sf::Text btnMode1, btnMode2, btnMode3, btnMode4;
    sf::Text btnSize1, btnSize2, btnSize3, btnSize4;
    sf::Text btnTime30, btnTime60, btnTimeInf;
    sf::Text btnBgBlack, btnBgBlue, btnBgGray;
    sf::Text btnChWhite, btnChRed, btnChGreen;
    sf::Text btnDdaOn, btnDdaOff, btnStartGame;
    sf::Text gameOverText, statsSummaryText, btnSaveYes, btnSaveNo, btnBackToMenu;

    sf::Texture najmanTex;
    sf::Sprite najmanSprite;
    sf::SoundBuffer najmanBuf;
    sf::Sound najmanSound;
    bool hasNajmanTex, hasNajmanSound;

    // ukladanie przyciskow na starcie
    void initMainMenuUI(float w, float h);
    void initGameOverUI(float w, float h);
};
