#include "GameEngine.h"
#include "StaticTarget.h"
#include "MovingTarget.h"
using namespace std;

// 1. Inicjalizacja InputManagera (ustawiamy czułość np. na 1.0 i pozycję startową na środek ekranu)
GameEngine::GameEngine()
    : window(sf::VideoMode::getDesktopMode(), "AimTrainer Pro", sf::Style::Fullscreen),
    inputManager(1.0f, sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f) {

    window.setFramerateLimit(144);
    window.setMouseCursorVisible(false);

    // 2. Wygląd celownika (mała, biała kropka o promieniu 4px)
    crosshair.setRadius(4.f);
    crosshair.setOrigin(4.f, 4.f); // Ustawiamy środek kropki
    crosshair.setFillColor(sf::Color::White);
}

void GameEngine::spawnTestTargets() {
    targets.push_back(make_unique<StaticTarget>(400.f, 300.f, 40.f));
    targets.push_back(make_unique<StaticTarget>(800.f, 500.f, 50.f));

    // SPAWNUJEMY TWOJĄ RUCHOMĄ TARCZĘ! (StartX, StartY, Promień, PrędkośćX, PrędkośćY)
    targets.push_back(make_unique<MovingTarget>(100.f, 100.f, 35.f, 150.f, 100.f));
}

void GameEngine::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Przelatujemy po tarczach. Jeśli trafisz, odpala się animacja kurczenia.
            for (auto& target : targets) {
                if (!target->isDead() && inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                    target->startShrinking();
                }
            }
        }
    }
}

void GameEngine::update(float deltaTime) {
    inputManager.update(window);
    crosshair.setPosition(inputManager.getX(), inputManager.getY());

    for (auto& target : targets) {
        target->update(deltaTime);
    }

    // Nowe czyszczenie pamięci: Usuwamy z kontenera tylko tarcze, które odtworzyły całą animację do końca
    targets.erase(remove_if(targets.begin(), targets.end(),
                            [](const unique_ptr<Target>& t) { return t->isDead(); }), targets.end());
}

void GameEngine::render() {
    window.clear(sf::Color::Black);
    for (auto& target : targets) {
        target->draw(window);
    }

    // 5. Rysowanie celownika (Zawsze na samym końcu, żeby był "na wierzchu" nad tarczami!)
    window.draw(crosshair);

    window.display();
}

void GameEngine::run() {
    spawnTestTargets();
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}
