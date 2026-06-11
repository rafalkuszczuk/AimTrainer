#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

// Główna klasa 'Target'. To klasa "abstrakcyjna" (taki szablon ogólny).
// Nie możemy w grze stworzyć po prostu "Targetu" - musimy zdefiniować, czy to Static, czy Moving Target.
class Target {
protected:
    // Słowo "protected" oznacza, że te zmienne są ukryte przed światem zewnętrznym,
    // ale klasy-dzieci (np. StaticTarget) mogą z nich swobodnie korzystać.
    float x, y, radius;     // Pozycja (x, y) i wielkość (promień) tarczy.
    bool shrinking = false; // Czy tarcza została trafiona i się kurczy?
    bool dead = false;      // Czy tarcza skurczyła się do końca i można ją usunąć?

    // Nowe zmienne dla trybu Survival
    float timeAlive = 0.0f;       // Ile czasu tarcza "żyje" na ekranie bez trafienia?
    bool isPenaltyTarget = false; // Czy to jest "zła" tarcza, w którą nie wolno strzelać?

public:
    // Konstruktor - podczas tworzenia tarczy od razu ustalamy jej miejsce i wielkość.
    Target(float startX, float startY, float r) : x(startX), y(startY), radius(r) {}

    // Wirtualny destruktor - wymagany, aby poprawnie kasować obiekty potomne z pamięci.
    virtual ~Target() = default;

    // Czysto wirtualne funkcje (to "= 0" na końcu).
    // Oznaczają one: "Ja jestem tylko szablonem! Każda klasa-dziecko musi sama napisać,
    // jak się aktualizuje (update), jak rysuje (draw) i zmienia kolor!".
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void setColor(sf::Color color) = 0; // Czysto wirtualna zmiana koloru

    // Standardowe Gettery (wyciągacze informacji)
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

    // Wywołanie tej funkcji odpala proces zapadania/zmniejszania tarczy.
    void startShrinking() { shrinking = true; }

    // Sprawdza, czy tarcza jest już gotowa na cmentarz.
    bool isDead() const { return dead; }

    // Gettery i Settery dla nowej logiki
    void addTimeAlive(float dt) { timeAlive += dt; } // Dodajemy czas, jaki upłynął (deltaTime)
    float getTimeAlive() const { return timeAlive; }

    void setPenalty(bool penalty) { isPenaltyTarget = penalty; } // Zmiana na tarczę-karę
    bool isPenalty() const { return isPenaltyTarget; }           // Czy to tarcza-kara?
};
