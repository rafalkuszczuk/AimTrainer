#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class InputManager {
private:
    float sensitivity; // Czułość myszki gracza
    float virtualX;    // Pozycja wirtualnego celownika na osi X (lewo/prawo)
    float virtualY;    // Pozycja wirtualnego celownika na osi Y (góra/dół)

public:
    // Konstruktor: ustala początkową czułość oraz punkt, od którego zaczyna celownik
    InputManager(float sens, float startX, float startY);

    void update(sf::RenderWindow& window);

    // Szybkie metody (gettery), żeby reszta gry wiedziała, gdzie jest celownik
    float getX() const;
    float getY() const;

    // Sprawdza, czy celownik "siedzi" na tarczy
    bool checkCollision(float targetX, float targetY, float targetRadius) const;

    // Funkcja pozwala na zmiane czulosci myszy w trakcie dzialania programu
    void setSensitivity(float newSens) {
        // Przypisanie nowej wartosci do wewnetrznego mnoznika predkosci celownika
        sensitivity = newSens;
    }
};
