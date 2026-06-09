#include "GameEngine.h"
#include "StaticTarget.h"

using namespace std;

GameEngine::GameEngine() : window(sf::VideoMode::getDesktopMode(), "AimTrainer Pro", sf::Style::Fullscreen) {
    window.setFramerateLimit(144);
    window.setMouseCursorVisible(false);
}

void GameEngine::spawnTestTargets() {
    // Wrzucamy do wektora jeden testowy cel na środek ekranu
    targets.push_back(make_unique<StaticTarget>(960.f, 549.f, 40.f));
}

void GameEngine::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Zdarzenie zamknięcia (np. Alt+F4)
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // ZABEZPIECZENIE: Zamykanie gry klawiszem ESC
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
}

void GameEngine::update(float deltaTime) {
    for (auto& target : targets) {
        target->update(deltaTime);
    }
}

void GameEngine::render() {
    window.clear(sf::Color::Black);
    for (auto& target : targets) {
        target->draw(window);
    }
    window.display();
}

void GameEngine::run() {
    spawnTestTargets();
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // Zliczanie czasu klatki
        processEvents();
        update(deltaTime);
        render();
    }
}

