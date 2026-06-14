/**
 * @file Target.h
 * @author Filip Krzyżkowiak
 * @brief Architektura silnika, inicjalizacja okna, główna pętla gry, zarządzanie pamięcią oraz polimorfizm obiektów.
 */
#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

// to jest glowny szablon dla wszystkich tarcz
class Target {
protected:
    // pozycja i wielkosc naszego kółka
    float x, y, radius;
    // czy wlasnie dostalo hita i znika
    bool shrinking = false;
    // czy juz zniknelo calkiem
    bool dead = false;

    // do trybu survival
    float timeAlive = 0.0f;       // ile sekund juz sobie zyje
    bool isPenaltyTarget = false; // czy to ta zielona tarcza (sojusznik)

public:
    // tworzymy tarcze na podanych kordach
    Target(float startX, float startY, float r) : x(startX), y(startY), radius(r) {}

    virtual ~Target() = default;

    // to musza miec wszystkie tarcze zeby dzialac
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setColor(sf::Color color) = 0;

    // wyciaganie danych
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

    // odpala animacje kurczenia po trafieniu
    void startShrinking() { shrinking = true; }

    // sprawdza czy juz mozna usunac tarcze z gry
    bool isDead() const { return dead; }

    // obsluga czasu i kar w survivalu
    void addTimeAlive(float dt) { timeAlive += dt; }
    float getTimeAlive() const { return timeAlive; }

    void setPenalty(bool penalty) { isPenaltyTarget = penalty; }
    bool isPenalty() const { return isPenaltyTarget; }
};
