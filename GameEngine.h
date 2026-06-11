#include "GameEngine.h"
#include "StaticTarget.h"
#include "MovingTarget.h"
#include <iostream>

using namespace std;

// Konstruktor silnika.
// To, co widzisz po dwukropku, to tzw. "Lista Inicjalizacyjna". To najszybszy i najbardziej
// poprawny sposób przypisywania wartości początkowych zmiennym w C++.
GameEngine::GameEngine()
    : currentState(STATE_MENU), playerName("Gracz"), sensString("1.0"), trackString("1.5"), activeField(0),
    window(sf::VideoMode::getDesktopMode(), "AimTrainer", sf::Style::Fullscreen), // Tworzymy okno na pełen ekran
    inputManager(1.0f, sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f),
    gameMode(1), lives(3), spawnTimer(0.0f),
    trackingTimeRequired(1.5f), currentTrackTime(0.0f),
    timeLimit(0.0f), elapsedTime(0.0f), targetSizeOption(2), useDDA(false), selectedTimeLimit(60.0f),
    reflexState(0), reflexWaitTime(0.0f), reflexTimer(0.0f), lastReactionTime(0.0f),
    mouseSensitivity(1.0f),
    currentBgColor(sf::Color(20, 20, 30)),
    currentChColor(sf::Color::White),
    gameOverSoundPlayed(false), isResultSaved(false),
    hasFont(false), hasCrosshairTex(false), hasShootSound(false), hasHitSound(false), hasDeadSound(false),
    hasNajmanTex(false), hasNajmanSound(false) {

    // Ograniczamy klatki do 144. Inaczej silnik mieliłby pętlę miliony razy na sekundę,
    // co przepalałoby procesor (CPU) i kartę graficzną (GPU).
    window.setFramerateLimit(144);

    // Inicjalizacja ziarna losowości na podstawie zegara sprzętowego komputera
    std::random_device rd;
    rng.seed(rd());

    // BEZPIECZNE ŁADOWANIE ZASOBÓW (System Fallback)
    // Co jeśli zapomnisz dać profesorowi pliku "arial.ttf"? W starej grze program po prostu by wybuchł (Crash).
    // Nasz if zabezpiecza to: jeśli pliku nie ma, flaga 'hasFont' pozostanie false i napisy nie będą wyświetlane, ale gra przetrwa.
    if (font.loadFromFile("arial.ttf")) {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        hasFont = true;
        initMainMenuUI();
        initGameOverUI();
    }

    crosshair.setRadius(4.f);
    crosshair.setOrigin(4.f, 4.f);
    if (crosshairTex.loadFromFile("crosshair.png")) {
        crosshair.setTexture(&crosshairTex);
        hasCrosshairTex = true;
    } else {
        crosshair.setFillColor(sf::Color::White); // Jak nie znajdzie celownika, po prostu narysuje białe koło!
    }

    // Ładujemy dźwięki. SetVolume obniża ich głośność na starcie, żeby "nie rozwaliły uszu" (szczególnie hit.wav z CS-a)
    if (shootBuf.loadFromFile("shoot.wav")) { shootSound.setBuffer(shootBuf); shootSound.setVolume(22.f); hasShootSound = true; }
    if (hitBuf.loadFromFile("hit.wav")) { hitSound.setBuffer(hitBuf); hitSound.setVolume(12.f); hasHitSound = true; }
    if (deadBuf.loadFromFile("dead.wav")) { deadSound.setBuffer(deadBuf); deadSound.setVolume(35.f); hasDeadSound = true; }

    if (najmanTex.loadFromFile("najman.png")) {
        najmanSprite.setTexture(najmanTex);
        // Matematyka do wyśrodkowania obrazka na ekranie: Środek_Ekranu - Połowa_Wielkości_Obrazka
        najmanSprite.setPosition(window.getSize().x / 2.0f - najmanTex.getSize().x / 2.0f, window.getSize().y / 2.0f - najmanTex.getSize().y / 2.0f);
        hasNajmanTex = true;
    }
    if (najmanBuf.loadFromFile("najman.wav")) { najmanSound.setBuffer(najmanBuf); hasNajmanSound = true; }
}

// Inicjalizacja Menu - funkcja organizacyjna. Po prostu przypisujemy właściwości każdemu napisowi.
void GameEngine::initMainMenuUI() {
    float midX = window.getSize().x / 2.0f; // Obliczamy X dla środka ekranu

    // Żeby użyć polskich znaków w kodzie (np. "Czułość"), musimy użyć prefiksu 'L' przed cudzysłowem
    // Informuje to kompilator C++, że tekst składa się z tzw. Wide Strings (szerokich znaków potrafiących pomieścić ą, ę, ł).
    menuTitle.setFont(font); menuTitle.setCharacterSize(42); menuTitle.setFillColor(sf::Color::Cyan);
    menuTitle.setString(L"AIM TRAINER - KONFIGURACJA");
    menuTitle.setPosition(midX - menuTitle.getGlobalBounds().width / 2.0f, 30.f);

    inputLabel.setFont(font); inputLabel.setCharacterSize(26); inputLabel.setFillColor(sf::Color::White);
    inputLabel.setPosition(midX - 450.f, 100.f);

    sensLabel.setFont(font); sensLabel.setCharacterSize(26); sensLabel.setFillColor(sf::Color::White);
    sensLabel.setPosition(midX - 100.f, 100.f);

    trackLabel.setFont(font); trackLabel.setCharacterSize(26); trackLabel.setFillColor(sf::Color::White);
    trackLabel.setPosition(midX + 250.f, 100.f);

    btnMode1.setFont(font); btnMode1.setCharacterSize(22); btnMode1.setPosition(midX - 400.f, 160.f);
    btnMode2.setFont(font); btnMode2.setCharacterSize(22); btnMode2.setPosition(midX - 150.f, 160.f);
    btnMode3.setFont(font); btnMode3.setCharacterSize(22); btnMode3.setPosition(midX + 100.f, 160.f);
    btnMode4.setFont(font); btnMode4.setCharacterSize(22); btnMode4.setPosition(midX + 350.f, 160.f);

    btnSize1.setFont(font); btnSize1.setCharacterSize(22); btnSize1.setPosition(midX - 350.f, 220.f);
    btnSize2.setFont(font); btnSize2.setCharacterSize(22); btnSize2.setPosition(midX - 150.f, 220.f);
    btnSize3.setFont(font); btnSize3.setCharacterSize(22); btnSize3.setPosition(midX + 50.f, 220.f);
    btnSize4.setFont(font); btnSize4.setCharacterSize(22); btnSize4.setPosition(midX + 250.f, 220.f);

    btnTime30.setFont(font); btnTime30.setCharacterSize(22); btnTime30.setPosition(midX - 250.f, 280.f);
    btnTime60.setFont(font); btnTime60.setCharacterSize(22); btnTime60.setPosition(midX - 50.f, 280.f);
    btnTimeInf.setFont(font); btnTimeInf.setCharacterSize(22); btnTimeInf.setPosition(midX + 150.f, 280.f);

    btnBgBlack.setFont(font); btnBgBlack.setCharacterSize(22); btnBgBlack.setPosition(midX - 250.f, 340.f);
    btnBgBlue.setFont(font); btnBgBlue.setCharacterSize(22); btnBgBlue.setPosition(midX - 50.f, 340.f);
    btnBgGray.setFont(font); btnBgGray.setCharacterSize(22); btnBgGray.setPosition(midX + 150.f, 340.f);

    btnChWhite.setFont(font); btnChWhite.setCharacterSize(22); btnChWhite.setPosition(midX - 300.f, 400.f);
    btnChRed.setFont(font); btnChRed.setCharacterSize(22); btnChRed.setPosition(midX - 50.f, 400.f);
    btnChGreen.setFont(font); btnChGreen.setCharacterSize(22); btnChGreen.setPosition(midX + 220.f, 400.f);

    btnDdaOn.setFont(font); btnDdaOn.setCharacterSize(22); btnDdaOn.setPosition(midX - 150.f, 480.f);
    btnDdaOff.setFont(font); btnDdaOff.setCharacterSize(22); btnDdaOff.setPosition(midX + 50.f, 480.f);

    btnStartGame.setFont(font); btnStartGame.setCharacterSize(34); btnStartGame.setFillColor(sf::Color::Green);
    btnStartGame.setString(L"[ URUCHOM TRENING ]");
    btnStartGame.setPosition(midX - btnStartGame.getGlobalBounds().width / 2.0f, 580.f);
}

void GameEngine::initGameOverUI() {
    float midX = window.getSize().x / 2.0f;

    gameOverText.setFont(font); gameOverText.setCharacterSize(46); gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString(L"KONIEC SESJI TRENINGOWEJ");
    gameOverText.setPosition(midX - gameOverText.getGlobalBounds().width / 2.0f, 50.f);

    statsSummaryText.setFont(font); statsSummaryText.setCharacterSize(24); statsSummaryText.setFillColor(sf::Color::Yellow);
    statsSummaryText.setPosition(midX - 300.f, 150.f);

    btnSaveYes.setFont(font); btnSaveYes.setCharacterSize(28); btnSaveYes.setFillColor(sf::Color::Green);
    btnSaveYes.setString(L"[ TAK - Zapisz wynik w pliku ]");
    btnSaveYes.setPosition(midX - 350.f, window.getSize().y - 180.f);

    btnSaveNo.setFont(font); btnSaveNo.setCharacterSize(28); btnSaveNo.setFillColor(sf::Color::Red);
    btnSaveNo.setString(L"[ NIE - Pomiń zapis ]");
    btnSaveNo.setPosition(midX + 50.f, window.getSize().y - 180.f);

    btnBackToMenu.setFont(font); btnBackToMenu.setCharacterSize(24); btnBackToMenu.setFillColor(sf::Color::White);
    btnBackToMenu.setString(L"<- Powrót do Menu (ESC)");
    btnBackToMenu.setPosition(50.f, window.getSize().y - 80.f);
}

// Resetuje wszystkie zmienne (stopery, liczniki, statystyki) przed każdym ponownym wejściem do gry
void GameEngine::resetGameSession() {
    targets.clear();
    stats = StatsManager();
    elapsedTime = 0.0f;
    spawnTimer = 0.0f;
    currentTrackTime = 0.0f;
    lives = 3;
    ddaMultiplier = 1.0f;
    gameOverSoundPlayed = false;
    isResultSaved = false;

    reflexState = 0;
    reflexTimer = 0.0f;
    lastReactionTime = 0.0f;

    // Próba zamiany tekstu ("stringa") wpisanego z klawiatury na liczbę ("float").
    // Konstrukcja try-catch zrzuca błędy (np. kiedy uczeń skasuje wszystko i naciśnie Enter).
    // Chroni to grę przed crashem i w awaryjnej sytuacji po prostu wraca do wartości 1.0.
    try {
        mouseSensitivity = std::stof(sensString);
        if (mouseSensitivity <= 0.0f) mouseSensitivity = 0.1f;
    } catch (...) {
        mouseSensitivity = 1.0f;
        sensString = "1.0";
    }
    inputManager.setSensitivity(mouseSensitivity);

    try {
        trackingTimeRequired = std::stof(trackString);
        if (trackingTimeRequired <= 0.0f) trackingTimeRequired = 1.5f;
    } catch (...) {
        trackingTimeRequired = 1.5f;
        trackString = "1.5";
    }
}

// Funkcja obsługująca KLIKANIE w teksty z menu.
// "getGlobalBounds()" tworzy matematyczny prostokąt oblekający nasz tekst z menu.
// "contains(mousePos)" pyta się komputera, czy współrzędne myszki mieszczą się w tym prostokącie.
void GameEngine::handleMenuClick(sf::Vector2f mousePos) {
    if (inputLabel.getGlobalBounds().contains(mousePos)) activeField = 0;
    if (gameMode != 4 && sensLabel.getGlobalBounds().contains(mousePos)) activeField = 1;
    if (gameMode == 3 && trackLabel.getGlobalBounds().contains(mousePos)) activeField = 2;

    if (btnMode1.getGlobalBounds().contains(mousePos)) gameMode = 1;
    if (btnMode2.getGlobalBounds().contains(mousePos)) gameMode = 2;
    if (btnMode3.getGlobalBounds().contains(mousePos)) gameMode = 3;
    if (btnMode4.getGlobalBounds().contains(mousePos)) { gameMode = 4; activeField = 0; }

    // W trybie 4 (Raw Reflex) wyłączamy pobieranie opcji o tarczach, bo to tryb bez tarcz!
    if (gameMode != 4) {
        if (btnSize1.getGlobalBounds().contains(mousePos)) targetSizeOption = 1;
        if (btnSize2.getGlobalBounds().contains(mousePos)) targetSizeOption = 2;
        if (btnSize3.getGlobalBounds().contains(mousePos)) targetSizeOption = 3;
        if (btnSize4.getGlobalBounds().contains(mousePos)) targetSizeOption = 4;

        if (btnTime30.getGlobalBounds().contains(mousePos)) selectedTimeLimit = 30.0f;
        if (btnTime60.getGlobalBounds().contains(mousePos)) selectedTimeLimit = 60.0f;
        if (btnTimeInf.getGlobalBounds().contains(mousePos)) selectedTimeLimit = 0.0f;

        if (btnBgBlack.getGlobalBounds().contains(mousePos)) currentBgColor = sf::Color::Black;
        if (btnBgBlue.getGlobalBounds().contains(mousePos)) currentBgColor = sf::Color(20, 20, 30);
        if (btnBgGray.getGlobalBounds().contains(mousePos)) currentBgColor = sf::Color(60, 60, 60);

        if (btnChWhite.getGlobalBounds().contains(mousePos)) currentChColor = sf::Color::White;
        if (btnChRed.getGlobalBounds().contains(mousePos)) currentChColor = sf::Color::Red;
        if (btnChGreen.getGlobalBounds().contains(mousePos)) currentChColor = sf::Color::Green;

        if (btnDdaOn.getGlobalBounds().contains(mousePos)) useDDA = true;
        if (btnDdaOff.getGlobalBounds().contains(mousePos)) useDDA = false;
    }

    if (btnStartGame.getGlobalBounds().contains(mousePos)) {
        resetGameSession();
        if (gameMode == 4) timeLimit = 0.0f;
        else timeLimit = selectedTimeLimit;

        currentState = STATE_PLAYING; // Zmieniamy stan maszyny, żeby odpalić grę!
    }
}

void GameEngine::handleGameOverClick(sf::Vector2f mousePos) {
    if (btnSaveYes.getGlobalBounds().contains(mousePos) && !isResultSaved) {
        stats.saveReport("wyniki.txt", playerName, gameMode);
        isResultSaved = true;
        btnSaveYes.setString(L"[ ZAPISANO POMYŚLNIE! ]");
        btnSaveYes.setFillColor(sf::Color::Transparent); // Ukrywamy napis TAK po kliknięciu
        btnSaveNo.setString(L"[ Kontynuuj ]");
        btnSaveNo.setFillColor(sf::Color::Green);
    }
    else if (btnSaveNo.getGlobalBounds().contains(mousePos) || btnBackToMenu.getGlobalBounds().contains(mousePos)) {
        currentState = STATE_MENU;
    }
}

// Funkcja przechwytująca eventy sprzętowe systemu operacyjnego
void GameEngine::processEvents() {
    sf::Event event;
    // Dopóki gra generuje jakieś akcje systemu (kliknięcia, ruszanie)
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // MECHANIZM PISANIA Z KLAWIATURY NA EKRANIE SFML
        if (currentState == STATE_MENU && event.type == sf::Event::TextEntered) {
            // Kod unicodowy 8 to fizyczny klawisz Backspace (cofanie i kasowanie znaków)
            if (event.text.unicode == 8) {
                if (activeField == 0 && !playerName.empty()) playerName.pop_back();
                else if (activeField == 1 && !sensString.empty()) sensString.pop_back();
                else if (activeField == 2 && !trackString.empty()) trackString.pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
                // Konwertujemy wciśnięty znak na normalną literę (char)
                char c = static_cast<char>(event.text.unicode);

                // Zabezpieczamy długość znaków i dozwolone przyciski dla różnych pól
                if (activeField == 0) {
                    if (playerName.length() < 12) playerName += c;
                } else if (activeField == 1) {
                    if (sensString.length() < 5 && (isdigit(c) || c == '.')) sensString += c;
                } else if (activeField == 2) {
                    if (trackString.length() < 5 && (isdigit(c) || c == '.')) trackString += c;
                }
            }
        }

        // MECHANIZM STRZELANIA (Kliknięcie Lewego Przycisku Myszy / LPM)
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Jeśli jesteśmy w Menu, klikamy w interfejs
            if (currentState == STATE_MENU) handleMenuClick(mousePos);
            else if (currentState == STATE_GAMEOVER) handleGameOverClick(mousePos);

            // Jeśli gra trwa, to kliknięcie oznacza STRZAŁ
            else if (currentState == STATE_PLAYING) {

                // STRZELANIE W TRYBIE 4 (Raw Reflex - bez tarcz, sam kolor tła)
                if (gameMode == 4) {
                    if (reflexState == 0) {
                        reflexState = 1;
                        reflexTimer = 0.0f;
                        std::uniform_real_distribution<float> distWait(1.5f, 4.0f);
                        reflexWaitTime = distWait(rng); // Zaskakuje gracza losowym czasem oczekiwania
                    }
                    else if (reflexState == 1) {
                        stats.addMiss(); // Fałszywy start! Kliknął przed zmianą koloru!
                        if (hasDeadSound) deadSound.play();
                        reflexState = 0;
                        lastReactionTime = -1.0f;
                    }
                    else if (reflexState == 2) {
                        stats.addHit(reflexTimer); // Sukces - dodaje do statystyk ułamek sekundy z timera
                        lastReactionTime = reflexTimer;
                        if (hasHitSound) hitSound.play();
                        reflexState = 0;
                    }
                }
                // STRZELANIE W TRYBACH FLICKING ORAZ SURVIVAL
                else if (gameMode != 3) {
                    if (hasShootSound) shootSound.play();
                    bool hitAnything = false;

                    // PRZESZUKIWANIE WEKTORA OD TYŁU: Dlaczego rbegin()?
                    // Bo tarcza wyrenderowana jako ostatnia, rysuje się na samym "wierzchu" innych tarcz.
                    // Klikając w zbiegowisko tarcz, chcemy trafić tylko tę najbardziej na wierzchu!
                    for (auto it = targets.rbegin(); it != targets.rend(); ++it) {
                        auto& target = *it;
                        // Metoda z polimorficznej klasy Target
                        if (!target->isDead() && inputManager.checkCollision(target->getX(), target->getY(), target->getRadius())) {
                            target->startShrinking(); // Odtwarza piękną animację znikania tarczy
                            hitAnything = true;
                            if (gameMode == 2 && target->isPenalty()) {
                                lives--;
                                if (hasDeadSound) deadSound.play();
                                if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zabiles swojego sojusznika (zielona tarcza!)");
                            } else {
                                if (hasHitSound) hitSound.play();
                                // Zapisujemy "Czas Życia" tarczy jako nasz realny czas reakcji od pojawienia do eliminacji!
                                stats.addHit(target->getTimeAlive());
                            }
                            break; // Jeśli strzeliliśmy jedną tarczę na wierzchu, przerywamy pętlę (kula nie przebija na wylot innych)
                        }
                    }

                    // Jeśli nie trafił żadnej tarczy z tablicy = PUDŁO
                    if (!hitAnything) {
                        stats.addMiss();
                        if (gameMode == 2) {
                            lives--;
                            if (hasDeadSound) deadSound.play();
                            if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Za duzo pudel (strzal w puste tlo)!");
                        }
                    }
                    updateDDA();
                }
            }
        }

        // Awaryjne wyjście do ekranu Game Over
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (currentState == STATE_PLAYING) currentState = STATE_GAMEOVER;
            else if (currentState == STATE_GAMEOVER) currentState = STATE_MENU;
            else if (currentState == STATE_MENU) window.close();
        }
    }
}

// System Dynamicznej Trudności (DDA - Dynamic Difficulty Adjustment)
// Mechanika z gier AAA typu Resident Evil. Analizuje postęp gracza po każdym strzale
// i zmienia trudność w locie (mnożnik ddaMultiplier zmniejszy lub powiększy promień tarczy).
void GameEngine::updateDDA() {
    if (!useDDA || gameMode == 3 || gameMode == 4) return;
    float acc = stats.getAccuracy();
    if (acc > 0.0f) {
        if (acc > 80.0f) ddaMultiplier = 0.8f;      // Idzie mu bardzo dobrze -> Tarcze mniejsze o 20%
        else if (acc < 50.0f) ddaMultiplier = 1.3f; // Gra słabo -> Tarcze większe o 30%
        else ddaMultiplier = 1.0f;
    }
}

// Fabryka tarcz do strzelania
void GameEngine::spawnRandomTarget() {
    // std::uniform_real_distribution to zaawansowana logika podająca losowe floaty ze wskazanego przedziału.
    std::uniform_real_distribution<float> distX(100.f, window.getSize().x - 100.f);
    std::uniform_real_distribution<float> distY(100.f, window.getSize().y - 100.f);
    float minR = 20.f, maxR = 70.f;

    if (targetSizeOption == 1) { minR = 15.f; maxR = 25.f; }
    else if (targetSizeOption == 2) { minR = 30.f; maxR = 45.f; }
    else if (targetSizeOption == 3) { minR = 50.f; maxR = 70.f; }

    std::uniform_real_distribution<float> distRadius(minR * ddaMultiplier, maxR * ddaMultiplier);
    float randX = distX(rng); float randY = distY(rng); float randR = distRadius(rng);

    // Deklarujemy inteligentny wskaźnik polimorficznego bazowego typu Target
    unique_ptr<Target> target;

    // WYMÓG PROJEKTOWY: Użycie Polimorfizmu (obiekt może przyjąć wiele form).
    // W zalezności od trybu gry wskaźnik buduje inną klasę (dziecko), a my wrzucamy ją
    // potem spokojnie do jednej tablicy "targets".
    if (gameMode == 3) {
        std::uniform_real_distribution<float> distVelocity(-400.f, 400.f);
        target = make_unique<MovingTarget>(randX, randY, randR, distVelocity(rng), distVelocity(rng), window.getSize().x, window.getSize().y);
    } else {
        target = make_unique<StaticTarget>(randX, randY, randR);
    }

    if (gameMode == 2) {
        std::uniform_int_distribution<int> chance(1, 100);
        if (chance(rng) <= 20) { target->setPenalty(true); target->setColor(sf::Color::Green); }
    }

    targets.push_back(std::move(target)); // "move" przekazuje wyłączne prawo posiadania inteligentnemu wskaźnikowi w wektorze!
}

// FUNKCJA UPDATE: To bije serce całego silnika. Przelicza matematykę ruchu i czasu.
// Parametr "deltaTime" (czas podany w ułamku sekundy, jaki zajęło komputerowi wyświetlenie pojedynczej klatki ekranu)
// to złota zasada programowania każdej gry wideo 2D i 3D: dzięki niemu ruch i upływ czasu oblicza się sprawiedliwie,
// i wygląda identycznie na starym laptopie (30 FPS) oraz potężnym monitorze (144 FPS).
void GameEngine::update(float deltaTime) {
    if (currentState == STATE_MENU) { window.setMouseCursorVisible(true); return; }

    if (currentState == STATE_GAMEOVER) {
        window.setMouseCursorVisible(true);
        if (!gameOverSoundPlayed && hasNajmanSound && gameMode == 2 && lives <= 0) {
            najmanSound.play();
            gameOverSoundPlayed = true;
        }
        return;
    }

    window.setMouseCursorVisible(gameMode == 4);
    elapsedTime += deltaTime; // Stoper czasu trwania rundy

    // Sprawdzanie czy koniec gry!
    if (timeLimit > 0.0f && elapsedTime >= timeLimit) { currentState = STATE_GAMEOVER; return; }
    if (gameMode == 2 && lives <= 0) { currentState = STATE_GAMEOVER; return; }

    // Mechanizm RAW Input wciąga dane bezpośrednio z matrycy myszki ignorując mnożniki Windowsa
    inputManager.update(window);
    crosshair.setPosition(inputManager.getX(), inputManager.getY());

    // Obliczanie stoperów trybu 4 (Bez tarcz)
    if (gameMode == 4) {
        if (reflexState == 1) {
            reflexTimer += deltaTime;
            if (reflexTimer >= reflexWaitTime) {
                reflexState = 2; // Czas minął, zmienia tło na ZIELONE
                reflexTimer = 0.0f;
            }
        }
        else if (reflexState == 2) {
            reflexTimer += deltaTime;
        }
        return;
    }

    // Tryb 3 bada zjawisko śledzenia obiektów. Jeśli najedziesz kursorem i go trzymasz, rośnie pasek currentTrackTime
    if (gameMode == 3) {
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
        spawnTimer += deltaTime;
        if (spawnTimer >= ((gameMode == 2 ? 1.0f : 0.7f) * ddaMultiplier)) { spawnRandomTarget(); spawnTimer = 0.0f; }
    }

    // Pętla odpalana dla każdej istniejącej tarczy na ekranie.
    // Zwiększa jej licznik życia i zarządza znikaniem (Flicking/Survival).
    for (auto& target : targets) {
        target->update(deltaTime); // WYMÓG PROJEKTU: POLIMORFIZM W AKCJI! (Funkcja wchodzi pod ten wskaźnik i wywołuje unikalny kod Ruchomej lub Statycznej Tarczy).
        target->addTimeAlive(deltaTime);

        if (!target->isDead()) {
            if (gameMode == 1 && target->getTimeAlive() > 3.0f) {
                target->startShrinking(); stats.addMiss(); target->addTimeAlive(-1000.f);
            }
            else if (gameMode == 2) {
                // Tarcze uciekają po 2 sekundach i zabierają życie.
                // -1000.f przy addTimeAlive to hackerska obrona silnika. Inaczej w następnej klatce (za 1/144 s) gra znów
                // zobaczyłaby na wpół skurczoną tarczę starszą niż 2.0s i zabrała nam KOLEJNE życie.
                if (!target->isPenalty() && target->getTimeAlive() > 2.0f) {
                    target->startShrinking(); lives--;
                    if (hasDeadSound) deadSound.play();
                    target->addTimeAlive(-1000.f);
                    if (lives <= 0 && stats.getDeathReason().empty()) stats.setDeathReason("Zbyt wolna reakcja - tarcza uciekla!");
                } else if (target->isPenalty() && target->getTimeAlive() > 2.5f) {
                    target->startShrinking();
                }
            }
        }
    }

    // SPRZĄTANIE PAMIĘCI (Zarządzanie Pamięcią i Unikanie Wycieków)
    // Idiom "Erase-Remove" usuwa z naszej kolekcji te tarcze, u których flaga isDead zwróciła wartość PRAWDA (Zniknęły lub Zestrzelone).
    targets.erase(remove_if(targets.begin(), targets.end(), [](const unique_ptr<Target>& t) { return t->isDead(); }), targets.end());
}

// Moduł Rysowania UI - dorysowuje warstwę tekstów (HUD) na "wierzch" ekranu bitowego
void GameEngine::drawUI() {
    if (!hasFont) return;

    string timeString = "Czas: " + to_string(static_cast<int>(elapsedTime)) + "s";
    if (timeLimit > 0.0f) timeString += " / " + to_string(static_cast<int>(timeLimit)) + "s";

    if (gameMode == 4) {
        uiText.setString(timeString);
        uiText.setPosition(20.f, 20.f);
        window.draw(uiText);

        sf::Text reflexInstruct;
        reflexInstruct.setFont(font);
        reflexInstruct.setCharacterSize(42);
        reflexInstruct.setFillColor(sf::Color::White);

        if (reflexState == 0) {
            if (lastReactionTime < 0.0f) reflexInstruct.setString(L"FAŁSZYWY START! Naciśnij lewy przycisk, aby spróbować ponownie.");
            else if (lastReactionTime > 0.0f) reflexInstruct.setString(sf::String(L"Wynik: ") + to_string(static_cast<int>(lastReactionTime * 1000.f)) + L" ms. Kliknij, by zagrać znowu.");
            else reflexInstruct.setString(L"Naciśnij lewy przycisk myszy, aby rozpocząć.");
        }
        else if (reflexState == 1) reflexInstruct.setString(L"Czekaj na zielony...");
        else if (reflexState == 2) reflexInstruct.setString(L"KLIKNIJ!");

        reflexInstruct.setPosition(window.getSize().x / 2.0f - reflexInstruct.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f);
        window.draw(reflexInstruct);
        return;
    }

    if (gameMode == 2) {
        uiText.setString(sf::String(L"Czas: ") + to_string(static_cast<int>(elapsedTime)) + L"s | Życia: ");
        uiText.setPosition(20.f, 20.f); window.draw(uiText);
        float textWidth = uiText.getGlobalBounds().width;
        // Pętla dynamicznie generująca grafiki serc
        for (int i = 0; i < lives; i++) {
            sf::CircleShape heart(15.f); heart.setFillColor(sf::Color::Red); heart.setPosition(30.f + textWidth + (i * 40.f), 20.f); window.draw(heart);
        }
    } else if (gameMode == 3) {
        uiText.setString(sf::String(L"Czas: ") + to_string(static_cast<int>(elapsedTime)) + L"s | Śledzenie: " + to_string(currentTrackTime).substr(0,4) + L"s / " + to_string(trackingTimeRequired).substr(0,4) + L"s");
        uiText.setPosition(20.f, 20.f); window.draw(uiText);
    } else {
        string ddaStr = useDDA ? (" | DDA: x" + to_string(ddaMultiplier).substr(0,3)) : " | DDA: OFF";
        uiText.setString(sf::String(L"Czas: ") + to_string(static_cast<int>(elapsedTime)) + L"s | Celność: " + to_string(static_cast<int>(stats.getAccuracy())) + L"%" + ddaStr);
        uiText.setPosition(20.f, 20.f); window.draw(uiText);
    }
}

// RENDER: Ostatni etap pętli silnika. Nakłada wyliczone obiekty i wysyła na ekran jako zbitą mapę bitową pikseli.
void GameEngine::render() {
    if (currentState == STATE_PLAYING && gameMode == 4) {
        if (reflexState == 0) window.clear(sf::Color(40, 40, 40));
        else if (reflexState == 1) window.clear(sf::Color(180, 0, 0));
        else if (reflexState == 2) window.clear(sf::Color(0, 180, 0));
    } else {
        window.clear(currentBgColor); // "Wyczyść" ekran przed narysowaniem nowej klatki nakładając wybrane tło
    }

    if (currentState == STATE_MENU) {
        inputLabel.setFillColor(activeField == 0 ? sf::Color::Yellow : sf::Color::White);
        inputLabel.setString(sf::String(L"Nick: ") + playerName + (activeField == 0 ? "_" : ""));

        btnMode1.setFillColor(gameMode == 1 ? sf::Color::Yellow : sf::Color::White); btnMode1.setString(gameMode == 1 ? L"> 1. Flicking <" : L"1. Flicking");
        btnMode2.setFillColor(gameMode == 2 ? sf::Color::Yellow : sf::Color::White); btnMode2.setString(gameMode == 2 ? L"> 2. Survival <" : L"2. Survival");
        btnMode3.setFillColor(gameMode == 3 ? sf::Color::Yellow : sf::Color::White); btnMode3.setString(gameMode == 3 ? L"> 3. Tracking <" : L"3. Tracking");
        btnMode4.setFillColor(gameMode == 4 ? sf::Color::Yellow : sf::Color::White); btnMode4.setString(gameMode == 4 ? L"> 4. Reflex <" : L"4. Reflex");

        window.draw(menuTitle); window.draw(inputLabel);
        window.draw(btnMode1); window.draw(btnMode2); window.draw(btnMode3); window.draw(btnMode4);

        if (gameMode != 4) {
            sensLabel.setFillColor(activeField == 1 ? sf::Color::Yellow : sf::Color::White);
            sensLabel.setString(sf::String(L"Czułość Myszki: ") + sensString + (activeField == 1 ? "_" : ""));
            window.draw(sensLabel);

            btnSize1.setFillColor(targetSizeOption == 1 ? sf::Color::Yellow : sf::Color::White); btnSize1.setString(L"Małe");
            btnSize2.setFillColor(targetSizeOption == 2 ? sf::Color::Yellow : sf::Color::White); btnSize2.setString(L"Średnie");
            btnSize3.setFillColor(targetSizeOption == 3 ? sf::Color::Yellow : sf::Color::White); btnSize3.setString(L"Duże");
            btnSize4.setFillColor(targetSizeOption == 4 ? sf::Color::Yellow : sf::Color::White); btnSize4.setString(L"Losowe");

            btnTime30.setFillColor(selectedTimeLimit == 30.0f ? sf::Color::Yellow : sf::Color::White); btnTime30.setString(L"Czas: 30s");
            btnTime60.setFillColor(selectedTimeLimit == 60.0f ? sf::Color::Yellow : sf::Color::White); btnTime60.setString(L"Czas: 60s");
            btnTimeInf.setFillColor(selectedTimeLimit == 0.0f ? sf::Color::Yellow : sf::Color::White); btnTimeInf.setString(L"Bez limitu");

            if (gameMode == 3) {
                trackLabel.setFillColor(activeField == 2 ? sf::Color::Yellow : sf::Color::White);
                trackLabel.setString(sf::String(L"Czas Śledzenia: ") + trackString + (activeField == 2 ? "_" : ""));
                window.draw(trackLabel);
            }

            btnBgBlack.setFillColor(currentBgColor == sf::Color::Black ? sf::Color::Yellow : sf::Color::White); btnBgBlack.setString(L"Tło: Czarne");
            btnBgBlue.setFillColor(currentBgColor == sf::Color(20, 20, 30) ? sf::Color::Yellow : sf::Color::White); btnBgBlue.setString(L"Tło: Niebieskie");
            btnBgGray.setFillColor(currentBgColor == sf::Color(60, 60, 60) ? sf::Color::Yellow : sf::Color::White); btnBgGray.setString(L"Tło: Szare");

            btnChWhite.setFillColor(currentChColor == sf::Color::White ? sf::Color::Yellow : sf::Color::White); btnChWhite.setString(L"Celownik: Biały");
            btnChRed.setFillColor(currentChColor == sf::Color::Red ? sf::Color::Yellow : sf::Color::White); btnChRed.setString(L"Celownik: Czerwony");
            btnChGreen.setFillColor(currentChColor == sf::Color::Green ? sf::Color::Yellow : sf::Color::White); btnChGreen.setString(L"Celownik: Zielony");

            btnDdaOn.setFillColor(useDDA ? sf::Color::Yellow : sf::Color::White); btnDdaOn.setString(L"DDA: Włączone");
            btnDdaOff.setFillColor(!useDDA ? sf::Color::Yellow : sf::Color::White); btnDdaOff.setString(L"DDA: Wyłączone");

            window.draw(btnSize1); window.draw(btnSize2); window.draw(btnSize3); window.draw(btnSize4);
            window.draw(btnTime30); window.draw(btnTime60); window.draw(btnTimeInf);
            window.draw(btnBgBlack); window.draw(btnBgBlue); window.draw(btnBgGray);
            window.draw(btnChWhite); window.draw(btnChRed); window.draw(btnChGreen);
            window.draw(btnDdaOn); window.draw(btnDdaOff);
        }

        window.draw(btnStartGame);
    }
    else if (currentState == STATE_GAMEOVER) {
        if (hasNajmanTex && gameMode == 2 && lives <= 0) window.draw(najmanSprite);

        // UŻYWAMY SF::STRING zamiast std::string (Wymóg C++ przy Unicode!)
        // Komputer przechowuje literę "Ł" w większym bajtowo pudełku. Biblioteka SFML daje nam klasę String,
        // która wie, jak to wszystko złączyć tak, żeby kompilator zdołał to skompilować, a na ekranie
        // nie pojawiły się krzaczki - śmieci pochodzące z wycieków pamięci.
        sf::String summary = sf::String(L"GRACZ: ") + playerName + sf::String(L"\n-------------------------------\n");
        summary += sf::String(L"Czas trwania sesji: ") + to_string(static_cast<int>(elapsedTime)) + sf::String(L" sekund\n");

        if (gameMode == 4) {
            summary += sf::String(L"Tryb: Raw Reflex\n");
            summary += sf::String(L"Udane próby: ") + to_string(stats.getHits()) + sf::String(L"\n");
            summary += sf::String(L"Fałszywe starty: ") + to_string(stats.getMisses()) + sf::String(L"\n");
            summary += sf::String(L"Średni czas reakcji: ") + to_string(static_cast<int>(stats.getAverageReactionTime() * 1000.f)) + sf::String(L" ms\n");
        }
        else if (gameMode == 3) {
            float trackTime = stats.getTrackingTime();
            float offTime = stats.getOffTargetTime();
            float totalTime = trackTime + offTime;
            float trackingAccuracy = (totalTime > 0) ? (trackTime / totalTime) * 100.0f : 0.0f;

            summary += sf::String(L"Tryb: Tracking\n");
            summary += sf::String(L"Zniszczone tarcze: ") + to_string(stats.getHits()) + sf::String(L"\n");
            summary += sf::String(L"Czas na tarczy: ") + to_string(trackTime).substr(0,4) + sf::String(L"s\n");
            summary += sf::String(L"Czas poza tarczą: ") + to_string(offTime).substr(0,4) + sf::String(L"s\n");
            summary += sf::String(L"Skuteczność śledzenia: ") + to_string(trackingAccuracy).substr(0,5) + sf::String(L"%\n");
        }
        else if (gameMode == 2) {
            summary += sf::String(L"Tryb: Survival\n");
            summary += sf::String(L"Zniszczone cele: ") + to_string(stats.getHits()) + sf::String(L"\n");
            summary += sf::String(L"Średni czas reakcji: ") + to_string(stats.getAverageReactionTime()).substr(0,4) + sf::String(L"s\n");
            if (!stats.getDeathReason().empty()) {
                summary += sf::String(L"Powód końca gry: ") + sf::String(stats.getDeathReason()) + sf::String(L"\n");
            }
        } else {
            summary += sf::String(L"Tryb: Flicking\n");
            summary += sf::String(L"Trafienia: ") + to_string(stats.getHits()) + sf::String(L" | Pudła: ") + to_string(stats.getMisses()) + sf::String(L"\n");
            summary += sf::String(L"Celność ogólna: ") + to_string(static_cast<int>(stats.getAccuracy())) + sf::String(L"%\n");
            summary += sf::String(L"Średni czas reakcji: ") + to_string(stats.getAverageReactionTime()).substr(0,4) + sf::String(L"s\n");
        }

        statsSummaryText.setString(summary);

        window.draw(gameOverText); window.draw(statsSummaryText); window.draw(btnSaveYes); window.draw(btnSaveNo); window.draw(btnBackToMenu);
    }
    else if (currentState == STATE_PLAYING) {
        if (gameMode != 4) {
            for (auto& target : targets) target->draw(window);
            // setFillColor na elemencie z teksturą działa w SFML jako "filtr". Po prostu barwi nasz celownik.
            crosshair.setFillColor(currentChColor);
            window.draw(crosshair);
        }
        drawUI();
    }

    window.display(); // "Wyrzuca" finalnie ułożoną bitmapę ekranu z bufora bezpośrednio na Twój monitor
}

// Główna pętla GameEngine (Game Loop). To ona utrzymuje grę przy życiu!
void GameEngine::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}
