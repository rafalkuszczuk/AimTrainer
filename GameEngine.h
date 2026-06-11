#pragma once // Zabezpieczenie przed wielokrotnym wczytaniem tego samego pliku przez kompilator
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

// MASZYNA STANÓW (State Machine)
// Gra nie składa się z wielu "okienek". Mamy jedno okno, a ten Enum definiuje,
// co aktualnie ma być w nim rysowane i jakie zasady w nim panują.
enum GameState { STATE_MENU, STATE_PLAYING, STATE_GAMEOVER };

class GameEngine {
private:
    GameState currentState; // Zmienna trzymająca nasz obecny stan (np. Menu)

    // Zmienne tekstowe do łapania znaków z klawiatury
    std::string playerName;  // Nick gracza
    std::string sensString;  // Czułość w formie tekstu (zanim zamienimy ją na ułamek float)
    std::string trackString; // Czas dla trybu Tracking (też jako tekst z klawiatury)
    int activeField;         // Wskazuje, co aktualnie kliknęliśmy i gdzie mamy pisać (0 - Nick, 1 - Sens, 2 - Tracking Time)

    // Główny obiekt okna gry w bibliotece SFML
    sf::RenderWindow window;

    // WYMÓG PROJEKTU: Polimorficzny kontener (jeden wektor trzymający różne typy tarcz).
    // unique_ptr to "inteligentny wskaźnik" - sam zwalnia pamięć RAM, kiedy tarcza jest usuwana z wektora!
    vector<unique_ptr<Target>> targets;

    InputManager inputManager; // Klasa ogarniająca ruch myszki
    sf::CircleShape crosshair; // Kształt naszego celownika (koło, na które nałożymy teksturę)

    StatsManager stats;        // Klasa licząca trafienia i pudła
    std::mt19937 rng;          // Nowoczesny generator liczb losowych w C++ (Mersenne Twister)

    // Główne zmienne napędzające mechanikę gry
    int gameMode;              // Wybrany tryb (1, 2, 3 lub 4)
    int lives;                 // Życia w trybie Survival
    float spawnTimer;          // Odlicza czas do pojawienia się nowej tarczy
    float trackingTimeRequired;// Ile sekund musimy trzymać celownik na tarczy
    float currentTrackTime;    // Aktualny postęp trzymania celownika
    float timeLimit;           // Globalny czas trwania rundy
    float elapsedTime;         // Stoper odliczający, ile rundy już minęło
    int targetSizeOption;      // Rozmiar tarcz (1-Małe, 2-Średnie, 3-Duże, 4-Losowe)
    bool useDDA;               // Czy system Dynamicznej Trudności jest włączony
    float selectedTimeLimit;   // Zmienna pomocnicza pamiętająca wybór z menu

    // --- ZMIENNE DLA TRYBU 4 (RAW REFLEX) ---
    // Ten tryb nie używa tarcz, tylko bada reakcję na zmianę koloru całego ekranu
    int reflexState;        // 0 - Start, 1 - Czerwony (czekaj), 2 - Zielony (klikaj!)
    float reflexWaitTime;   // Wylosowany czas czekania (żeby gracz nie klikał na pamięć)
    float reflexTimer;      // Stoper mierzący czas reakcji na zielone światło
    float lastReactionTime; // Pamięta ostatni wynik, żeby pokazać go pośrodku ekranu

    // Konfiguracja sprzętu
    float mouseSensitivity; // Czułość myszy
    sf::Color currentBgColor; // Aktualny kolor tła
    sf::Color currentChColor; // Aktualny kolor celownika

    // Zmienne zasobów (SFML potrzebuje osobno bufora trzymającego plik .wav, a osobno odtwarzacza sf::Sound)
    sf::Texture crosshairTex;
    bool hasCrosshairTex; // Flaga "Fallback" (Zabezpieczenie przed błędem, gdy plik grafiki zostanie usunięty z dysku)
    sf::SoundBuffer shootBuf, hitBuf, deadBuf;
    sf::Sound shootSound, hitSound, deadSound;
    bool hasShootSound, hasHitSound, hasDeadSound;
    float ddaMultiplier = 1.0f; // Mnożnik trudności (zmienia rozmiar i tempo spawnowania)

    // Zmienne dla ekranu końcowego i "Easter Egga" (Marcin Najman po porażce)
    bool gameOverSoundPlayed;
    sf::Texture najmanTex;
    sf::Sprite najmanSprite; // Sprite to "obrazek" powiązany z teksturą, który można narysować na ekranie
    sf::SoundBuffer najmanBuf;
    sf::Sound najmanSound;
    bool hasNajmanTex, hasNajmanSound;
    bool isResultSaved;      // Zabezpiecza, żeby gracz nie zapisał wyniku 10 razy z rzędu klikając w przycisk

    // Obiekty typu sf::Text pozwalają rysować napisy w oknie SFML.
    // Żeby móc ich używać, musimy załadować plik z czcionką (sf::Font).
    sf::Font font;
    sf::Text uiText; // Tekst wyświetlający czas i punkty podczas gry (HUD)
    bool hasFont;

    // Teksty do Menu Głównego. Przedrostek 'btn' oznacza "Przycisk" (z ang. button).
    // Będą one działać jak przyciski, bo w kodzie będziemy sprawdzać, czy kliknięto w ich obszar.
    sf::Text menuTitle, inputLabel, sensLabel, trackLabel;
    sf::Text btnMode1, btnMode2, btnMode3, btnMode4;
    sf::Text btnSize1, btnSize2, btnSize3, btnSize4;
    sf::Text btnTime30, btnTime60, btnTimeInf;
    sf::Text btnTrack10, btnTrack15, btnTrack20;
    sf::Text btnBgBlack, btnBgBlue, btnBgGray;
    sf::Text btnChWhite, btnChRed, btnChGreen;
    sf::Text btnDdaOn, btnDdaOff;
    sf::Text btnStartGame;

    // Teksty do ekranu końcowego
    sf::Text gameOverText, statsSummaryText, btnSaveYes, btnSaveNo, btnBackToMenu;

    // Główne funkcje napędzające tzw. "Game Loop" (Pętlę Gry)
    void processEvents();            // Zbieranie kliknięć i wciśnięć klawiszy (Klawiatura, Mysz)
    void update(float deltaTime);    // Obliczanie logiki i fizyki wewnątrz gry
    void render();                   // Rysowanie policzonych rzeczy na karcie graficznej

    void spawnRandomTarget();        // Funkcja tworząca nowe tarcze
    void drawUI();                   // Funkcja rysująca napisy z punktami w trakcie gry
    void updateDDA();                // Funkcja analizująca punkty i zmieniająca trudność

    // Funkcje pomocnicze, które tylko organizują i przygotowują interfejs
    void initMainMenuUI();
    void initGameOverUI();
    void handleMenuClick(sf::Vector2f mousePos);
    void handleGameOverClick(sf::Vector2f mousePos);
    void resetGameSession();

public:
    GameEngine(); // Konstruktor domyślny klasy
    void run();   // Publiczna funkcja odpalana w main.cpp (uruchamia cały silnik)
};
