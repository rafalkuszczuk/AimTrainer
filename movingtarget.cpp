#include "MovingTarget.h"

using namespace std;

MovingTarget::MovingTarget(float startX, float startY, float r, float vX, float vY, float screenW, float screenH)
    : Target(startX, startY, r), maxX(screenW), maxY(screenH) {

    velocityX = vX; // Ustawienie prędkości lotu kółka
    velocityY = vY;

    shape.setRadius(radius);
    shape.setOrigin(radius, radius); // Znowu ustawiamy punkt wewnątrz, centralnie.
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red); // Ruchome cele, żeby się wyróżniały, są czerwone.
}

void MovingTarget::setColor(sf::Color color) {
    shape.setFillColor(color);
}

void MovingTarget::update(float deltaTime) {
    // Ruch: przesuwamy X i Y dodając do nich prędkość i czas klatki
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Zderzenie w poziomie (X)
    // "x - radius" to lewy boczek tarczy. Jeżeli przebije 0 (lewa ściana okna)...
    if (x - radius <= 0) {
        x = radius; // ...to wpychamy je lekko z powrotem, żeby nie ugrzęzło w ścianie...
        velocityX = -velocityX; // ...i odwracamy wektor ruchu na minus! (leciał lewo -> leci prawo).

        // Jeżeli prawy bok (x + radius) walnie w prawą granicę okna (maxX)...
    } else if (x + radius >= maxX) {
        x = maxX - radius;
        velocityX = -velocityX; // Znowu odbicie jak piłeczka (leciał prawo -> leci w lewo)
    }

    // Zderzenie w pionie (Y) działa DOKŁADNIE tak samo, tylko sprawdzamy sufit i podłogę.
    if (y - radius <= 0) {
        y = radius;
        velocityY = -velocityY; // Odwrócenie wektora Y (góra/dół)
    } else if (y + radius >= maxY) {
        y = maxY - radius;
        velocityY = -velocityY;
    }

    // Zapisujemy nową pozycję dla "grafiki" tarczy
    shape.setPosition(x, y);

    // Identyczna animacja umierania i znikania, co w StaticTarget
    if (shrinking) {
        radius -= 250.0f * deltaTime;
        if (radius <= 0) {
            radius = 0;
            dead = true;
        } else {
            shape.setRadius(radius);
            shape.setOrigin(radius, radius);
        }
    }
}

void MovingTarget::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
