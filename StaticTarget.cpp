#include "StaticTarget.h"

using namespace std;

// Konstruktor - przypisujemy pozycję i wielkość startową z szablonu bazowego (Target)
StaticTarget::StaticTarget(float startX, float startY, float r) : Target(startX, startY, r) {
    shape.setRadius(radius);

    // Ustawiamy punkt "chwytu" koła w samym jego środku, a nie w lewym-górnym rogu.
    // To kluczowe, żeby koło kurczyło się do środka, a nie przesuwało się w lewo-górę!
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);

    shape.setFillColor(sf::Color::Blue); // Statyczny cel na start będzie miał kolor niebieski
}

void StaticTarget::update(float deltaTime) {
    // Odpala się, jeśli cel dostał hita z pistoletu
    if (shrinking) {
        // Tłumaczenie dla małpki:
        // Zmniejszamy promień o 250 pikseli w każdej sekundzie.
        // Mnożymy to przez "deltaTime" (czas od wyrysowania ostatniej klatki obrazu).
        // Dzięki temu nieważne czy masz 30, 60 czy 300 FPS - tarcza kurczy się w takim samym tempie.
        radius -= 250.0f * deltaTime;

        // Jeśli promień spadł do zera (albo niżej)...
        if (radius <= 0) {
            radius = 0;
            dead = true; // Koniec animacji, tarcza "umarła", można ją usunąć z wektora obiektów.
        } else {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius); // Zmiana origin by tarcza cały czas kurczyła się idealnie do środka
        }
    }
}

void StaticTarget::draw(sf::RenderWindow& window) {
    window.draw(shape); // Po prostu maluje nam koło w odpowiednim miejscu
}

void StaticTarget::setColor(sf::Color color) {
    shape.setFillColor(color); // Pozwala ręcznie zmienić kolor tarczy (np. z niebieskiego na fioletowy)
}
