#include "InputManager.h"

using namespace std;

InputManager::InputManager(float sens, float startX, float startY) {
    sensitivity = sens;
    virtualX = startX; // Startowa pozycja celownika (najpewniej środek ekranu)
    virtualY = startY;
}

void InputManager::update(sf::RenderWindow& window) {
    // Upewniamy się, że gra jest aktywnym oknem, by nie "kraść" myszy systemowi.
    // (Zabezpieczenie przed tym, żeby myszka nie wariowała, gdy klikniesz Alt-Tab!)
    if (!window.hasFocus()) return;

    // Patrzymy, gdzie system operacyjny widzi teraz naszą myszkę.
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();

    // Wyznaczamy idealny środek okna gry.
    sf::Vector2i center(windowSize.x / 2, windowSize.y / 2);

    // Obliczamy deltę (różnicę) przesunięcia od środka okna.
    // Tłumaczenie dla małpki: Sprawdzamy "jak bardzo wychyliliśmy myszkę w stosunku do punktu centralnego?".
    float deltaX = static_cast<float>(mousePos.x - center.x);
    float deltaY = static_cast<float>(mousePos.y - center.y);

    // Aplikujemy czułość niczym w CS2 (Sensitivity).
    // Przesuwamy nasz WIRTUALNY celownik o wychylenie pomnożone przez naszą czułość.
    virtualX += deltaX * sensitivity;
    virtualY += deltaY * sensitivity;

    // Blokowanie wirtualnego celownika, by nie odleciał w kosmos poza ekran.
    // Jeśli wyjedzie za lewą krawędź (< 0), to zatrzymujemy go na zerze. Analogicznie dla reszty krawędzi.
    if (virtualX < 0) virtualX = 0;
    if (virtualX > windowSize.x) virtualX = windowSize.x;
    if (virtualY < 0) virtualY = 0;
    if (virtualY > windowSize.y) virtualY = windowSize.y;

    // Tłumaczenie dla małpki (IMITACJA RAW INPUT):
    // Tu dzieje się magia strzelanek. Żeby kursora nam nie zabrakło (np. uderzając w skraj monitora),
    // po przesunięciu celownika WIRTUALNEGO, bierzemy fizyczny kursor myszy i teleportujemy
    // go brutalnie z powrotem na sam środek okna.
    sf::Mouse::setPosition(center, window);
}

float InputManager::getX() const { return virtualX; }
float InputManager::getY() const { return virtualY; }

bool InputManager::checkCollision(float targetX, float targetY, float targetRadius) const {
    // Odległość w poziomie (X) i pionie (Y) między naszym celownikiem, a środkiem tarczy.
    float dx = virtualX - targetX;
    float dy = virtualY - targetY;

    // Tłumaczenie dla małpki (Matematyka bez bólu):
    // Optymalizacja: normalnie do liczenia dystansu między dwoma kropkami używa się
    // trudnego pierwiastka (sqrt). Pierwiastkowanie bardzo męczy procesor komputera.
    // Zamiast tego z Twierdzenia Pitagorasa (a^2 + b^2 = c^2) sprawdzamy sam kwadrat odległości.
    // Pytamy komputer: "czy kwadrat Twojej odległości jest mniejszy niż kwadrat promienia tarczy?".
    // Efekt jest identyczny, a komputer się nie poci.
    float distanceSquared = (dx * dx) + (dy * dy);
    float radiusSquared = targetRadius * targetRadius;

    // Jeśli odległość (do kwadratu) jest mniejsza lub równa promieniowi (do kwadratu), to znaczy,
    // że celownik wpada w środek koła. Zwracamy TRUE - mamy trafienie!
    return distanceSquared <= radiusSquared;
}
