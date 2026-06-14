/**
 * @file GameEngine.cpp
 * @author Filip Krzyżkowiak
 * @brief Architektura silnika, inicjalizacja okna, główna pętla gry, zarządzanie pamięcią oraz polimorfizm obiektów.
 */
#include "GameEngine.h"
#include "StaticTarget.h"
#include "MovingTarget.h"
#include <iostream>

using namespace std;

GameEngine::GameEngine()
    : currentState(STATE_MENU),
    window(sf::VideoMode::getDesktopMode(), "AimTrainer", sf::Style::Fullscreen),
    inputManager(1.0f, sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f),
    lives(3), spawnTimer(0.0f), trackingTimeRequired(1.5f), currentTrackTime(0.0f),
    timeLimit(0.0f), elapsedTime(0.0f), reflexState(0), reflexWaitTime(0.0f), reflexTimer(0.0f),
    lastReactionTime(0.0f), mouseSensitivity(1.0f), ddaMultiplier(1.0f),
    gameOverSoundPlayed(false), isResultSaved(false),
    hasCrosshairTex(false), hasShootSound(false), hasHitSound(false), hasDeadSound(false) {

    // max fps zeby nam kompa nie spalilo
    window.setFramerateLimit(144);
    std::random_device rd;
    rng.seed(rd());

    // ladujemy napisy i obrazki menu
    ui.loadResources(window.getSize().x, window.getSize().y);

    crosshair.setRadius(4.f);
    crosshair.setOrigin(4.f, 4.f);

    // probujemy wczytac grafike celownika
    if (crosshairTex.loadFromFile("crosshair.png")) {
        crosshair.setTexture(&crosshairTex);
        hasCrosshairTex = true;
    } else {
        crosshair.setFillColor(sf::Color::White); // jak nie ma pliku to dajemy biala kropke
    }

    // ladowanie dzwiekow
    if (shootBuf.loadFromFile("shoot.wav")) { shootSound.setBuffer(shootBuf); shootSound.setVolume(22.f); hasShootSound = true; }
    if (hitBuf.loadFromFile("hit.wav")) { hitSound.setBuffer(hitBuf); hitSound.setVolume(12.f); hasHitSound = true; }
    if (deadBuf.loadFromFile("dead.wav")) { deadSound.setBuffer(deadBuf); deadSound.setVolume(35.f); hasDeadSound = true; }
}

void GameEngine::resetGameSession() {
    // czyscimy wszystko przed nowa gierka
    targets.clear();
    stats = StatsManager();
    elapsedTime = 0.0f; spawnTimer = 0.0f; currentTrackTime = 0.0f;
    lives = 3; ddaMultiplier = 1.0f;
    gameOverSoundPlayed = false; isResultSaved = false;
    reflexState = 0; reflexTimer = 0.0f; lastReactionTime = 0.0f;

    // naprawa naszego bledu z brakiem mozliwosci zapisu!
    ui.resetSaveUI();

    // czytanie wpisanej czulosci
    try {
        mouseSensitivity = std::stof(ui.sensString);
        if (mouseSensitivity <= 0.0f) mouseSensitivity = 0.1f;
    } catch (...) {
        mouseSensitivity = 1.0f; ui.sensString = "1.0";
    }
    inputManager.setSensitivity(mouseSensitivity);

    // czytanie wpisanego czasu sledzenia
    try {
        trackingTimeRequired = std::stof(ui.trackString);
        if (trackingTimeRequired <= 0.0f) trackingTimeRequired = 1.5f;
    } catch (...) {
        trackingTimeRequired = 1.5f; ui.trackString = "1.5";
    }
}

void GameEngine::processEvents() {
    sf::Event event;
    // sprawdzamy wszystko co uzytkownik klika
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // pisanie na klawiaturze w menu
        if (currentState == STATE_MENU && event.type == sf::Event::TextEntered) {
            ui.handleTextInput(event.text.unicode);
        }

        // lewy przycisk myszy
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // co klikamy w menu
            if (currentState == STATE_MENU) {
                ui.handleMenuClick(mousePos);
                // jak klikniesz start, to zerujemy sesje i jazda
                if (ui.startGameRequested) {
                    resetGameSession();
                    timeLimit = (ui.gameMode == 4) ? 0.0f : ui.selectedTimeLimit;
                    currentState = STATE_PLAYING;
                    ui.startGameRequested = false;
                }
            }
            // co klikamy po przegranej
            else if (currentState == STATE_GAMEOVER) {
                ui.handleGameOverClick(mousePos, stats, isResultSaved);
                if (ui.backToMenuRequested) {
                    currentState = STATE_MENU;
                    ui.backToMenuRequested = false;
                }
            }
            // co klikamy grajac
            else if (currentState == STATE_PLAYING) {
                if (ui.gameMode == 4) {
                    // tryb refleksu - sprawdzanie stanow swiatelek
                    if (reflexState == 0) {
                        reflexState = 1; reflexTimer = 0.0f;
                        std::uniform_real_distribution<float> distWait(1.5f, 4.0f);
                        reflexWaitTime = distWait(rng);
                    }
                    else if (reflexState == 1) { // kliknal za wczesnie
                        stats.addMiss(); if (hasDeadSound) deadSound.play();
                        reflexState = 0; lastReactionTime = -1.0f;
                    }
                    else if (reflexState == 2) { // kliknal na zielonym
                        stats.addHit(reflexTimer); lastReactionTime = reflexTimer;
                        if (hasHitSound) hitSound.play();
                        reflexState = 0;
                    }
                }
                else if (ui.gameMode != 3) {
                    // pif paf
                    if (hasShootSound) shootSound.play();
                    bool hitAnything = false;

                    // lecimy od konca zeby trafiac w kółka co sa na wierzchu
                    for (auto it = targets.rbegin(); it != targets.rend(); ++it) {
                        auto& target = *it;
                        // trafilismy!
                        if (!target->isDead() && inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                            target->startShrinking(); hitAnything = true;
                            // trafil w zielonego, ojoj
                            if (ui.gameMode == 2 && target->isPenalty()) {
                                lives--; if (hasDeadSound) deadSound.play();
                                if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zabiles swojego sojusznika (zielona tarcza!)");
                            } else {
                                if (hasHitSound) hitSound.play(); stats.addHit(target->getTimeAlive());
                            }
                            break;
                        }
                    }
                    // strzal w tlo
                    if (!hitAnything) {
                        stats.addMiss();
                        if (ui.gameMode == 2) {
                            lives--; if (hasDeadSound) deadSound.play();
                            if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Za duzo pudel (strzal w puste tlo)!");
                        }
                    }
                    // aktualizujemy trudnosc bo moze byc za latwo
                    updateDDA();
                }
            }
        }
        // wciska ESC
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (currentState == STATE_PLAYING) currentState = STATE_GAMEOVER;
            else if (currentState == STATE_GAMEOVER) currentState = STATE_MENU;
            else if (currentState == STATE_MENU) window.close();
        }
    }
}

void GameEngine::updateDDA() {
    // ignorujemy jak wylaczone
    if (!ui.useDDA || ui.gameMode == 3 || ui.gameMode == 4) return;
    float acc = stats.getAccuracy();
    if (acc > 0.0f) {
        // krecenie mnoznikiem trudnosci
        if (acc > 80.0f) ddaMultiplier = 0.8f; // zmniejszamy rozmiary i czas = trudniej
        else if (acc < 50.0f) ddaMultiplier = 1.3f; // zwiekszamy = latwiej
        else ddaMultiplier = 1.0f; // zostaje normalka
    }
}

void GameEngine::spawnRandomTarget() {
    // maszyna losujaca kordy
    std::uniform_real_distribution<float> distX(100.f, window.getSize().x - 100.f);
    std::uniform_real_distribution<float> distY(100.f, window.getSize().y - 100.f);
    float minR = 20.f, maxR = 70.f;

    // wybieranie wielkosci tarcz z menu
    if (ui.targetSizeOption == 1) { minR = 15.f; maxR = 25.f; }
    else if (ui.targetSizeOption == 2) { minR = 30.f; maxR = 45.f; }
    else if (ui.targetSizeOption == 3) { minR = 50.f; maxR = 70.f; }

    // losujemy dokladny rozmiar uwzgledniajac nasz system DDA
    std::uniform_real_distribution<float> distRadius(minR * ddaMultiplier, maxR * ddaMultiplier);
    float randX = distX(rng); float randY = distY(rng); float randR = distRadius(rng);

    unique_ptr<Target> target;

    // tryb latajacy
    if (ui.gameMode == 3) {
        std::uniform_real_distribution<float> distVelocity(-400.f, 400.f);
        target = make_unique<MovingTarget>(randX, randY, randR, distVelocity(rng), distVelocity(rng), window.getSize().x, window.getSize().y);
    } else { // zwykly
        target = make_unique<StaticTarget>(randX, randY, randR);
    }

    // robimy falszywe tarcze jak to survival
    if (ui.gameMode == 2) {
        std::uniform_int_distribution<int> chance(1, 100);
        if (chance(rng) <= 20) { target->setPenalty(true); target->setColor(sf::Color::Green); }
    }
    // dodajemy do naszego wektora
    targets.push_back(std::move(target));
}

void GameEngine::update(float deltaTime) {
    if (currentState == STATE_MENU) { window.setMouseCursorVisible(true); return; }

    // jak gra sie skonczyla
    if (currentState == STATE_GAMEOVER) {
        window.setMouseCursorVisible(true);
        // puszczamy najmana jak przegrales w survivalu
        if (!gameOverSoundPlayed && ui.gameMode == 2 && lives <= 0) {
            ui.playNajmanSound();
            gameOverSoundPlayed = true;
        }
        return;
    }

    // ukrywamy myszke zeby sam celownik zostal
    window.setMouseCursorVisible(ui.gameMode == 4);
    elapsedTime += deltaTime;

    // koniec czasu lub zyc
    if (timeLimit > 0.0f && elapsedTime >= timeLimit) { currentState = STATE_GAMEOVER; return; }
    if (ui.gameMode == 2 && lives <= 0) { currentState = STATE_GAMEOVER; return; }

    // ruszamy naszym celownikiem
    inputManager.update(window);
    crosshair.setPosition(inputManager.getX(), inputManager.getY());

    // logika trybu z refleksu (zmiany kolorow ekranu z czasem)
    if (ui.gameMode == 4) {
        if (reflexState == 1) {
            reflexTimer += deltaTime;
            if (reflexTimer >= reflexWaitTime) { reflexState = 2; reflexTimer = 0.0f; }
        }
        else if (reflexState == 2) reflexTimer += deltaTime;
        return;
    }

    // logika trybu trzymania na tarczy
    if (ui.gameMode == 3) {
        bool isHovering = false;
        for (auto& target : targets) {
            if (!target->isDead()) {
                if (inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                    isHovering = true; currentTrackTime += deltaTime; stats.addTrackingTime(deltaTime); target->setColor(sf::Color::Yellow);
                    if (currentTrackTime >= trackingTimeRequired) {
                        if (hasHitSound) hitSound.play(); target->startShrinking(); stats.addHit(0.0f); currentTrackTime = 0.0f;
                    }
                } else { target->setColor(sf::Color::Red); stats.addOffTargetTime(deltaTime); }
            }
        }
        if (!isHovering) currentTrackTime = 0.0f;
        if (targets.empty() || (targets.size() == 1 && targets[0]->isDead())) spawnRandomTarget();
    } else {
        // logika wyrzucania nowych tarcz na ekran w pozostalych trybach
        spawnTimer += deltaTime;
        if (spawnTimer >= ((ui.gameMode == 2 ? 1.0f : 0.7f) * ddaMultiplier)) { spawnRandomTarget(); spawnTimer = 0.0f; }
    }

    for (auto& target : targets) {
        target->update(deltaTime); target->addTimeAlive(deltaTime);

        if (!target->isDead()) {
            // w zwyklym trybie tarcza psuje sie po 3 sekundach
            if (ui.gameMode == 1 && target->getTimeAlive() > 3.0f) {
                target->startShrinking(); stats.addMiss(); target->addTimeAlive(-1000.f);
            }
            // w survivalu to cie zabija
            else if (ui.gameMode == 2) {
                if (!target->isPenalty() && target->getTimeAlive() > 2.0f) {
                    target->startShrinking(); lives--; if (hasDeadSound) deadSound.play(); target->addTimeAlive(-1000.f);
                    if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zbyt wolna reakcja - tarcza uciekla!");
                    // sojusznicy moga bezpiecznie uciec
                } else if (target->isPenalty() && target->getTimeAlive() > 2.5f) {
                    target->startShrinking();
                }
            }
        }
    }
    // wywalamy do kosza z pamieci te tarcze, ktore sa "dead"
    targets.erase(remove_if(targets.begin(), targets.end(), [](const unique_ptr<Target>& t) { return t->isDead(); }), targets.end());
}

void GameEngine::render() {
    // kolorki w tle dla roznych trybow
    if (currentState == STATE_PLAYING && ui.gameMode == 4) {
        if (reflexState == 0) window.clear(sf::Color(40, 40, 40));
        else if (reflexState == 1) window.clear(sf::Color(180, 0, 0));
        else if (reflexState == 2) window.clear(sf::Color(0, 180, 0));
    } else {
        window.clear(ui.currentBgColor);
    }

    // proste rysowanie w zaleznosci co sie dzieje
    if (currentState == STATE_MENU) {
        ui.drawMenu(window);
    }
    else if (currentState == STATE_GAMEOVER) {
        ui.drawGameOver(window, elapsedTime, stats, lives);
    }
    else if (currentState == STATE_PLAYING) {
        if (ui.gameMode != 4) {
            for (auto& target : targets) target->draw(window);
            crosshair.setFillColor(ui.currentChColor);
            window.draw(crosshair);
            ui.drawHUD(window, elapsedTime, timeLimit, lives, currentTrackTime, trackingTimeRequired, ddaMultiplier, stats);
        } else {
            ui.drawReflexInstruct(window, reflexState, lastReactionTime);
        }
    }
    window.display(); // pokazujemy graczowi klatke obrazu
}

void GameEngine::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // ile minelo od ostatniej klatki
        processEvents();
        update(deltaTime);
        render();
    }
}
