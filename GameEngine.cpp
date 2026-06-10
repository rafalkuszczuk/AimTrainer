#include "GameEngine.h"
#include "StaticTarget.h"
#include "MovingTarget.h"
#include <iostream>

using namespace std;

GameEngine::GameEngine(string name, int mode, float trackReq, float limit, int sizeOpt, bool enableDDA)
    : playerName(name),
    window(sf::VideoMode::getDesktopMode(), "AimTrainer Pro", sf::Style::Fullscreen),
    inputManager(1.0f, sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f),
    gameMode(mode), lives(3), spawnTimer(0.0f),
    trackingTimeRequired(trackReq), currentTrackTime(0.0f),
    timeLimit(limit), elapsedTime(0.0f), targetSizeOption(sizeOpt), useDDA(enableDDA), // <--- PRZYPISANIE TUTAJ
    isGameOver(false), gameOverSoundPlayed(false),
    hasFont(false), hasCrosshairTex(false), hasShootSound(false), hasHitSound(false), hasDeadSound(false),
    hasNajmanTex(false), hasNajmanSound(false) {

    window.setFramerateLimit(144);
    window.setMouseCursorVisible(false);

    std::random_device rd;
    rng.seed(rd());

    if (!font.loadFromFile("arial.ttf")) {
        cout << "[INFO] Brak arial.ttf. Teksty UI beda wylaczone." << endl;
    } else {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        hasFont = true;
    }

    crosshair.setRadius(8.f);
    crosshair.setOrigin(8.f, 8.f);
    if (crosshairTex.loadFromFile("crosshair.png")) {
        crosshair.setTexture(&crosshairTex);
        hasCrosshairTex = true;
    } else {
        crosshair.setFillColor(sf::Color::White);
    }

    // --- ŁADOWANIE AUDIO ---
    if (shootBuf.loadFromFile("shoot.wav")) {
        shootSound.setBuffer(shootBuf);
        hasShootSound = true;
    }
    if (hitBuf.loadFromFile("hit.wav")) {
        hitSound.setBuffer(hitBuf);
        hasHitSound = true;
    }
    if (deadBuf.loadFromFile("dead.wav")) {
        deadSound.setBuffer(deadBuf);
        hasDeadSound = true;
    }

    // --- ŁADOWANIE NAJMANA ---
    if (najmanTex.loadFromFile("najman.png")) {
        najmanSprite.setTexture(najmanTex);
        najmanSprite.setPosition(
            window.getSize().x / 2.0f - najmanTex.getSize().x / 2.0f,
            window.getSize().y / 2.0f - najmanTex.getSize().y / 2.0f
            );
        hasNajmanTex = true;
    }
    if (najmanBuf.loadFromFile("najman.wav")) {
        najmanSound.setBuffer(najmanBuf);
        hasNajmanSound = true;
    }
}

void GameEngine::updateDDA() {
    // Od razu przerywamy, jeśli gracz wyłączył DDA lub jesteśmy w trybie Tracking
    if (!useDDA || gameMode == 3) return;

    float acc = stats.getAccuracy();
    if (acc > 0.0f) {
        if (acc > 80.0f) ddaMultiplier = 0.8f;
        else if (acc < 50.0f) ddaMultiplier = 1.3f;
        else ddaMultiplier = 1.0f;
    }
}

void GameEngine::spawnRandomTarget() {
    std::uniform_real_distribution<float> distX(100.f, window.getSize().x - 100.f);
    std::uniform_real_distribution<float> distY(100.f, window.getSize().y - 100.f);

    float minR = 20.f, maxR = 70.f;
    if (targetSizeOption == 1) { minR = 15.f; maxR = 25.f; }
    else if (targetSizeOption == 2) { minR = 30.f; maxR = 45.f; }
    else if (targetSizeOption == 3) { minR = 50.f; maxR = 70.f; }

    std::uniform_real_distribution<float> distRadius(minR * ddaMultiplier, maxR * ddaMultiplier);

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
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (isGameOver) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
            continue;
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

        if (gameMode != 3 && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (hasShootSound) shootSound.play();

            bool hitAnything = false;
            for (auto it = targets.rbegin(); it != targets.rend(); ++it) {
                auto& target = *it;
                if (!target->isDead() && inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                    target->startShrinking();
                    hitAnything = true;

                    if (gameMode == 2 && target->isPenalty()) {
                        lives--;
                        if (hasDeadSound) deadSound.play(); // Dźwięk utraty serca!
                        if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zabita przez ZIELONA pulapke!");
                    } else {
                        if (hasHitSound) hitSound.play(); // Dźwięk trafienia
                        stats.addHit(target->getTimeAlive());
                    }
                    break;
                }
            }

            if (!hitAnything) {
                stats.addMiss();
                if (gameMode == 2) {
                    lives--;
                    if (hasDeadSound) deadSound.play(); // Dźwięk utraty serca za pudło!
                    if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Za duzo pudelek (strzal w puste tlo)!");
                }
            }
            updateDDA();
        }
    }
}

void GameEngine::update(float deltaTime) {
    if (isGameOver) {
        if (!gameOverSoundPlayed && hasNajmanSound) {
            najmanSound.play();
            gameOverSoundPlayed = true;
        }
        return;
    }

    elapsedTime += deltaTime;
    if (timeLimit > 0.0f && elapsedTime >= timeLimit) {
        window.close();
        return;
    }

    if (gameMode == 2 && lives <= 0) {
        isGameOver = true;
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
                        if (hasHitSound) hitSound.play();
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
        if (targets.empty() || (targets.size() == 1 && targets[0]->isDead())) spawnRandomTarget();

    } else {
        spawnTimer += deltaTime;
        float actualCooldown = ((gameMode == 2) ? 1.0f : 0.7f) * ddaMultiplier;

        if (spawnTimer >= actualCooldown) {
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
                if (hasDeadSound) deadSound.play(); // Dźwięk utraty serca!

                target->addTimeAlive(-1000.0f);

                if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zbyt wolna reakcja - tarcza uciekla!");
            } else if (target->isPenalty() && target->getTimeAlive() > 2.5f) {
                target->startShrinking();
            }
        }
    }

    targets.erase(remove_if(targets.begin(), targets.end(), [](const unique_ptr<Target>& t) { return t->isDead(); }), targets.end());
}

void GameEngine::drawUI() {
    if (!hasFont) return;

    string timeString = "Czas: " + to_string(static_cast<int>(elapsedTime)) + "s";
    if (timeLimit > 0.0f) timeString += " / " + to_string(static_cast<int>(timeLimit)) + "s";

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
        // Dynamiczny tekst DDA
        string ddaStr = useDDA ? (" | DDA: x" + to_string(ddaMultiplier).substr(0,3)) : " | DDA: OFF";
        uiText.setString(timeString + " | Celnosc: " + to_string(static_cast<int>(stats.getAccuracy())) + "%" + ddaStr);
        uiText.setPosition(20.f, 20.f);
        window.draw(uiText);
    }
}

void GameEngine::render() {
    window.clear(sf::Color::Black);

    if (isGameOver) {
        if (hasNajmanTex) window.draw(najmanSprite);
        if (hasFont) {
            uiText.setString("GAME OVER\nWcisnij ESC aby wrocic do statystyk");
            uiText.setPosition(window.getSize().x / 2.0f - 200.f, 50.f);
            window.draw(uiText);
        }
        window.display();
        return;
    }

    for (auto& target : targets) target->draw(window);
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
    stats.saveReport("wyniki.txt", playerName, gameMode); // <-- Twój Scoreboard w pełnej krasie!
    stats.printReportConsole(gameMode);
}
