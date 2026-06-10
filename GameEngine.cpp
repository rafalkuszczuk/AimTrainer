#include "GameEngine.h"
#include "StaticTarget.h"
#include "MovingTarget.h"
#include <iostream>

using namespace std;

GameEngine::GameEngine(int mode, float trackReq, float limit, int sizeOpt)
    : window(sf::VideoMode::getDesktopMode(), "AimTrainer Pro", sf::Style::Fullscreen),
    inputManager(1.0f, sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f),
    gameMode(mode), lives(3), spawnTimer(0.0f),
    trackingTimeRequired(trackReq), currentTrackTime(0.0f),
    timeLimit(limit), elapsedTime(0.0f), targetSizeOption(sizeOpt) {

    window.setFramerateLimit(144);
    window.setMouseCursorVisible(false);

    crosshair.setRadius(4.f);
    crosshair.setOrigin(4.f, 4.f);
    crosshair.setFillColor(sf::Color::White);

    std::random_device rd;
    rng.seed(rd());

    if (!font.loadFromFile("arial.ttf")) {
        cout << "[OSTRZEZENIE] Brak arial.ttf" << endl;
    } else {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
    }
}

void GameEngine::spawnRandomTarget() {
    std::uniform_real_distribution<float> distX(100.f, window.getSize().x - 100.f);
    std::uniform_real_distribution<float> distY(100.f, window.getSize().y - 100.f);

    // LOGIKA WYBORU ROZMIARU
    float minR = 20.f, maxR = 70.f;
    if (targetSizeOption == 1) { minR = 15.f; maxR = 25.f; }       // Małe
    else if (targetSizeOption == 2) { minR = 30.f; maxR = 45.f; }  // Średnie
    else if (targetSizeOption == 3) { minR = 50.f; maxR = 70.f; }  // Duże
    else { minR = 20.f; maxR = 70.f; }                             // Losowe

    std::uniform_real_distribution<float> distRadius(minR, maxR);

    float randX = distX(rng);
    float randY = distY(rng);
    float randR = distRadius(rng);

    unique_ptr<Target> target;

    if (gameMode == 3) {
        std::uniform_real_distribution<float> distVelocity(-400.f, 400.f);
        target = make_unique<MovingTarget>(randX, randY, randR, distVelocity(rng), distVelocity(rng), window.getSize().x, window.getSize().y);
    } else {
        target = make_unique<StaticTarget>(randX, randY, randR);
    }

    if (gameMode == 2) {
        std::uniform_int_distribution<int> chance(1, 100);
        if (chance(rng) <= 20) {
            target->setPenalty(true);
            target->setColor(sf::Color::Green);
        }
    }

    targets.push_back(std::move(target));
}

void GameEngine::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }

        if (gameMode != 3 && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            bool hitAnything = false;

            for (auto it = targets.rbegin(); it != targets.rend(); ++it) {
                auto& target = *it;

                if (!target->isDead() && inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                    target->startShrinking();
                    hitAnything = true;

                    if (gameMode == 2 && target->isPenalty()) {
                        lives--;
                        // LOGIKA ŚMIERCI: Pułapka
                        if (lives <= 0 && stats.getDeathReason().empty()) {
                            stats.setDeathReason("Zabity przez ZIELONA pulapke!");
                        }
                    } else {
                        stats.addHit(target->getTimeAlive());
                    }
                    break;
                }
            }

            if (!hitAnything) {
                stats.addMiss();
                if (gameMode == 2) {
                    lives--;
                    // LOGIKA ŚMIERCI: Pudło
                    if (lives <= 0 && stats.getDeathReason().empty()) {
                        stats.setDeathReason("Za duzo pudel (strzalow w puste tlo)!");
                    }
                }
            }
        }
    }
}

void GameEngine::update(float deltaTime) {
    elapsedTime += deltaTime;

    if (timeLimit > 0.0f && elapsedTime >= timeLimit) {
        window.close();
        return;
    }

    if (gameMode == 2 && lives <= 0) {
        window.close();
        return;
    }

    inputManager.update(window);
    crosshair.setPosition(inputManager.getX(), inputManager.getY());

    if (gameMode == 3) {
        bool isHovering = false;
        for (auto& target : targets) {
            if (!target->isDead()) {
                if (inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                    isHovering = true;
                    currentTrackTime += deltaTime;
                    stats.addTrackingTime(deltaTime);
                    target->setColor(sf::Color::Yellow);

                    if (currentTrackTime >= trackingTimeRequired) {
                        target->startShrinking();
                        stats.addHit(0.0f);
                        currentTrackTime = 0.0f;
                    }
                } else {
                    target->setColor(sf::Color::Red);
                    stats.addOffTargetTime(deltaTime);
                }
            }
        }
        if (!isHovering) currentTrackTime = 0.0f;

        if (targets.empty() || (targets.size() == 1 && targets[0]->isDead())) {
            spawnRandomTarget();
        }
    } else {
        spawnTimer += deltaTime;
        float spawnCooldown = (gameMode == 2) ? 1.0f : 0.7f;
        if (spawnTimer >= spawnCooldown) {
            spawnRandomTarget();
            spawnTimer = 0.0f;
        }
    }

    for (auto& target : targets) {
        target->update(deltaTime);
        target->addTimeAlive(deltaTime);

        if (gameMode == 2 && !target->isDead()) {
            if (!target->isPenalty() && target->getTimeAlive() > 2.0f) {
                target->startShrinking();
                lives--;
                // LOGIKA ŚMIERCI: Brak czasu
                if (lives <= 0 && stats.getDeathReason().empty()) {
                    stats.setDeathReason("Zbyt wolna reakcja - tarcza uciekla!");
                }
            } else if (target->isPenalty() && target->getTimeAlive() > 2.5f) {
                target->startShrinking();
            }
        }
    }

    targets.erase(remove_if(targets.begin(), targets.end(),
                            [](const unique_ptr<Target>& t) { return t->isDead(); }), targets.end());
}



void GameEngine::drawUI() {
    string timeString = "Czas: " + to_string(static_cast<int>(elapsedTime)) + "s";
    if (timeLimit > 0.0f) {
        timeString += " / " + to_string(static_cast<int>(timeLimit)) + "s";
    }

    if (gameMode == 2) {
        uiText.setString(timeString + " | Zycia: ");
        uiText.setPosition(20.f, 20.f);
        window.draw(uiText);

        float textWidth = uiText.getGlobalBounds().width;
        for (int i = 0; i < lives; i++) {
            sf::CircleShape heart(15.f);
            heart.setFillColor(sf::Color::Red);
            heart.setPosition(30.f + textWidth + (i * 40.f), 20.f);
            window.draw(heart);
        }
    } else if (gameMode == 3) {
        uiText.setString(timeString + " | Sledzenie: " + to_string(currentTrackTime).substr(0,4) + "s / " + to_string(trackingTimeRequired).substr(0,4) + "s");
        uiText.setPosition(20.f, 20.f);
        window.draw(uiText);
    } else {
        uiText.setString(timeString + " | Celnosc: " + to_string(static_cast<int>(stats.getAccuracy())) + "%");
        uiText.setPosition(20.f, 20.f);
        window.draw(uiText);
    }
}

void GameEngine::render() {
    window.clear(sf::Color::Black);
    for (auto& target : targets) {
        target->draw(window);
    }
    drawUI();
    window.draw(crosshair);
    window.display();
}

void GameEngine::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }

    stats.saveReport("wyniki.txt");
    stats.printReportConsole(gameMode);
}
