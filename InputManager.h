/**
 * @file InputManager.h
 * @author Rafał Kuszczuk
 * @brief Zaawansowana interakcja, "raw input", matematyka detekcji kolizji (hitboxy) i wektory ruchu.
 */
#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class InputManager {
private:
    float sensitivity; // czułość myszki gracza
    float virtualX;    // pozycja wirtualnego celownika na osi X (lewo/prawo)
    float virtualY;    // pozycja wirtualnego celownika na osi Y (góra/dół)

public:
    // konstruktor ustala początkową czułość oraz punkt, od którego zaczyna celownik
    InputManager(float sens, float startX, float startY);

    void update(sf::RenderWindow& window);

    // szybkie metody (gettery), żeby reszta gry wiedziała, gdzie jest celownik
    float getX() const;
    float getY() const;

    // sprawdza, czy celownik "siedzi" na tarczy
    bool checkCollision(float targetX, float targetY, float targetRadius) const;

    // Funkcja pozwala na zmiane czulosci myszy w trakcie dzialania programu
    void setSensitivity(float newSens) {
        // Przypisanie nowej wartosci do wewnetrznego mnoznika predkosci celownika
        sensitivity = newSens;
    }
};
