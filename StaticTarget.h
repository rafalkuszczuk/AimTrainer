/**
 * @file [Nazwa_Pliku]
 * @author Rafał Kuszczuk
 * @brief Zaawansowana interakcja, implementacja "raw input", matematyka detekcji kolizji oraz wektory ruchu.
 */
#pragma once
#include "Target.h"

using namespace std;

// Klasa StaticTarget dziedziczy po szablonie Target. Oznacza to, że jest zwykłym, stojącym celem.
class StaticTarget : public Target {
private:
    sf::CircleShape shape; // Konkretny kształt (koło) rysowany na ekranie przez bibliotekę SFML

public:
    StaticTarget(float startX, float startY, float r);

    // Zobowiązaliśmy się w klasie Target napisać własną logikę tych funkcji (tzw. nadpisywanie = override)
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void setColor(sf::Color color) override;
};
