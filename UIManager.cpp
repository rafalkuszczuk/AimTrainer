/**
 * @file UIManager.cpp
 * @author Sebastian Mielcarek
 * @brief Logika UI, implementacja algorytmu DDA, zapis analityki do plików (I/O) oraz wczytywanie zasobów.
 */
#include "UIManager.h"
#include <iostream>

using namespace std;

UIManager::UIManager()
    : playerName("Gracz"), sensString("1.0"), trackString("1.5"), activeField(0),
    gameMode(1), targetSizeOption(2), selectedTimeLimit(60.0f), useDDA(false),
    currentBgColor(sf::Color(20, 20, 30)), currentChColor(sf::Color::White),
    startGameRequested(false), backToMenuRequested(false),
    hasFont(false), hasNajmanTex(false), hasNajmanSound(false) {
}

void UIManager::loadResources(float width, float height) {
    // wczytywanie czcionki
    if (font.loadFromFile("arial.ttf")) {
        uiText.setFont(font);
        uiText.setCharacterSize(24);
        uiText.setFillColor(sf::Color::White);
        hasFont = true;
        initMainMenuUI(width, height);
        initGameOverUI(width, height);
    }

    // wczytywanie obrazka potężnego wlodarza
    if (najmanTex.loadFromFile("najman.png")) {
        najmanSprite.setTexture(najmanTex);
        najmanSprite.setPosition(width / 2.0f - najmanTex.getSize().x / 2.0f, height / 2.0f - najmanTex.getSize().y / 2.0f);
        hasNajmanTex = true;
    }
    if (najmanBuf.loadFromFile("najman.wav")) {
        najmanSound.setBuffer(najmanBuf);
        hasNajmanSound = true;
    }
}

void UIManager::initMainMenuUI(float w, float h) {
    // to jest po prostu matematyka zeby guziki byly rowno, omijamy zeby sie nie zanudzic
    float midX = w / 2.0f;
    float startY = h * 0.12f;
    float stepY = h * 0.08f;
    float col = w * 0.12f;

    menuTitle.setFont(font); menuTitle.setCharacterSize(42); menuTitle.setFillColor(sf::Color::Cyan);
    menuTitle.setString(L"AIM TRAINER - KONFIGURACJA");
    menuTitle.setPosition(midX - menuTitle.getGlobalBounds().width / 2.0f, h * 0.04f);

    inputLabel.setFont(font); inputLabel.setCharacterSize(26); inputLabel.setFillColor(sf::Color::White);
    inputLabel.setPosition(midX - col * 2.5f, startY);
    sensLabel.setFont(font); sensLabel.setCharacterSize(26); sensLabel.setFillColor(sf::Color::White);
    sensLabel.setPosition(midX - col * 0.5f, startY);
    trackLabel.setFont(font); trackLabel.setCharacterSize(26); trackLabel.setFillColor(sf::Color::White);
    trackLabel.setPosition(midX + col * 1.5f, startY);

    float row2 = startY + stepY;
    btnMode1.setFont(font); btnMode1.setCharacterSize(22); btnMode1.setPosition(midX - col * 2.5f, row2);
    btnMode2.setFont(font); btnMode2.setCharacterSize(22); btnMode2.setPosition(midX - col * 1.0f, row2);
    btnMode3.setFont(font); btnMode3.setCharacterSize(22); btnMode3.setPosition(midX + col * 0.5f, row2);
    btnMode4.setFont(font); btnMode4.setCharacterSize(22); btnMode4.setPosition(midX + col * 2.0f, row2);

    float row3 = row2 + stepY;
    btnSize1.setFont(font); btnSize1.setCharacterSize(22); btnSize1.setPosition(midX - col * 2.0f, row3);
    btnSize2.setFont(font); btnSize2.setCharacterSize(22); btnSize2.setPosition(midX - col * 0.8f, row3);
    btnSize3.setFont(font); btnSize3.setCharacterSize(22); btnSize3.setPosition(midX + col * 0.4f, row3);
    btnSize4.setFont(font); btnSize4.setCharacterSize(22); btnSize4.setPosition(midX + col * 1.6f, row3);

    float row4 = row3 + stepY;
    btnTime30.setFont(font); btnTime30.setCharacterSize(22); btnTime30.setPosition(midX - col * 1.5f, row4);
    btnTime60.setFont(font); btnTime60.setCharacterSize(22); btnTime60.setPosition(midX - col * 0.2f, row4);
    btnTimeInf.setFont(font); btnTimeInf.setCharacterSize(22); btnTimeInf.setPosition(midX + col * 1.1f, row4);

    float row5 = row4 + stepY;
    btnBgBlack.setFont(font); btnBgBlack.setCharacterSize(22); btnBgBlack.setPosition(midX - col * 1.5f, row5);
    btnBgBlue.setFont(font); btnBgBlue.setCharacterSize(22); btnBgBlue.setPosition(midX - col * 0.2f, row5);
    btnBgGray.setFont(font); btnBgGray.setCharacterSize(22); btnBgGray.setPosition(midX + col * 1.1f, row5);

    float row6 = row5 + stepY;
    btnChWhite.setFont(font); btnChWhite.setCharacterSize(22); btnChWhite.setPosition(midX - col * 1.8f, row6);
    btnChRed.setFont(font); btnChRed.setCharacterSize(22); btnChRed.setPosition(midX - col * 0.2f, row6);
    btnChGreen.setFont(font); btnChGreen.setCharacterSize(22); btnChGreen.setPosition(midX + col * 1.4f, row6);

    float row7 = row6 + stepY;
    btnDdaOn.setFont(font); btnDdaOn.setCharacterSize(22); btnDdaOn.setPosition(midX - col * 1.0f, row7);
    btnDdaOff.setFont(font); btnDdaOff.setCharacterSize(22); btnDdaOff.setPosition(midX + col * 0.5f, row7);

    btnStartGame.setFont(font); btnStartGame.setCharacterSize(34); btnStartGame.setFillColor(sf::Color::Green);
    btnStartGame.setString(L"[ URUCHOM TRENING ]");
    btnStartGame.setPosition(midX - btnStartGame.getGlobalBounds().width / 2.0f, h * 0.85f);
}

void UIManager::initGameOverUI(float w, float h) {
    // to samo co wyzej, tylko z ekranem po smierci
    float midX = w / 2.0f;
    gameOverText.setFont(font); gameOverText.setCharacterSize(46); gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString(L"KONIEC SESJI TRENINGOWEJ");
    gameOverText.setPosition(midX - gameOverText.getGlobalBounds().width / 2.0f, h * 0.08f);

    statsSummaryText.setFont(font); statsSummaryText.setCharacterSize(24); statsSummaryText.setFillColor(sf::Color::Yellow);
    statsSummaryText.setPosition(w * 0.35f, h * 0.20f);

    btnSaveYes.setFont(font); btnSaveYes.setCharacterSize(28); btnSaveYes.setFillColor(sf::Color::Green);
    btnSaveYes.setString(L"[ TAK - Zapisz wynik w pliku ]");
    btnSaveYes.setPosition(midX - (w * 0.2f), h - (h * 0.2f));

    btnSaveNo.setFont(font); btnSaveNo.setCharacterSize(28); btnSaveNo.setFillColor(sf::Color::Red);
    btnSaveNo.setString(L"[ NIE - Pomiń zapis ]");
    btnSaveNo.setPosition(midX + (w * 0.05f), h - (h * 0.2f));

    btnBackToMenu.setFont(font); btnBackToMenu.setCharacterSize(24); btnBackToMenu.setFillColor(sf::Color::White);
    btnBackToMenu.setString(L"<- Powrót do Menu (ESC)");
    btnBackToMenu.setPosition(w * 0.05f, h - (h * 0.1f));
}

void UIManager::handleTextInput(sf::Uint32 unicode) {
    // wcisniecie backspace (kasowanie z klawiatury)
    if (unicode == 8) {
        if (activeField == 0 && !playerName.empty()) playerName.pop_back();
        else if (activeField == 1 && !sensString.empty()) sensString.pop_back();
        else if (activeField == 2 && !trackString.empty()) trackString.pop_back();
        // wpisywanie normalnych znaczkow i cyferek
    } else if (unicode < 128 && unicode >= 32) {
        char c = static_cast<char>(unicode);
        if (activeField == 0 && playerName.length() < 12) playerName += c;
        else if (activeField == 1 && sensString.length() < 5 && (isdigit(c) || c == '.')) sensString += c;
        else if (activeField == 2 && trackString.length() < 5 && (isdigit(c) || c == '.')) trackString += c;
    }
}

void UIManager::handleMenuClick(sf::Vector2f mousePos) {
    // sprawdzamy co kliknal uzytkownik jak na kalkulatorze i podmieniamy opcje
    if (inputLabel.getGlobalBounds().contains(mousePos)) activeField = 0;
    if (gameMode != 4 && sensLabel.getGlobalBounds().contains(mousePos)) activeField = 1;
    if (gameMode == 3 && trackLabel.getGlobalBounds().contains(mousePos)) activeField = 2;

    if (btnMode1.getGlobalBounds().contains(mousePos)) gameMode = 1;
    if (btnMode2.getGlobalBounds().contains(mousePos)) gameMode = 2;
    if (btnMode3.getGlobalBounds().contains(mousePos)) gameMode = 3;
    if (btnMode4.getGlobalBounds().contains(mousePos)) { gameMode = 4; activeField = 0; }

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

    if (btnStartGame.getGlobalBounds().contains(mousePos)) startGameRequested = true;
}

void UIManager::handleGameOverClick(sf::Vector2f mousePos, StatsManager& stats, bool& isResultSaved) {
    // klikniecie "zapisz wynik"
    if (btnSaveYes.getGlobalBounds().contains(mousePos) && !isResultSaved) {
        stats.saveReport("wyniki.txt", playerName, gameMode);
        isResultSaved = true; // blokujemy zapis zeby ktos nie nabil 100 takich samych
        btnSaveYes.setString(L"[ ZAPISANO POMYŚLNIE! ]");
        btnSaveYes.setFillColor(sf::Color::Transparent); // zeby wydawalo sie nieklikalne
        btnSaveNo.setString(L"[ Kontynuuj ]");
        btnSaveNo.setFillColor(sf::Color::Green);
    }
    // ucieczka do menu
    else if (btnSaveNo.getGlobalBounds().contains(mousePos) || btnBackToMenu.getGlobalBounds().contains(mousePos)) {
        backToMenuRequested = true;
    }
}

void UIManager::drawMenu(sf::RenderWindow& window) {
    // podswietla na zolto to, w co mozemy pisac
    inputLabel.setFillColor(activeField == 0 ? sf::Color::Yellow : sf::Color::White);
    inputLabel.setString(sf::String(L"Nick: ") + playerName + (activeField == 0 ? "_" : ""));

    // podswietla na zolto wybrane tryby i opcje
    btnMode1.setFillColor(gameMode == 1 ? sf::Color::Yellow : sf::Color::White); btnMode1.setString(gameMode == 1 ? L"> 1. Flicking <" : L"1. Flicking");
    btnMode2.setFillColor(gameMode == 2 ? sf::Color::Yellow : sf::Color::White); btnMode2.setString(gameMode == 2 ? L"> 2. Survival <" : L"2. Survival");
    btnMode3.setFillColor(gameMode == 3 ? sf::Color::Yellow : sf::Color::White); btnMode3.setString(gameMode == 3 ? L"> 3. Tracking <" : L"3. Tracking");
    btnMode4.setFillColor(gameMode == 4 ? sf::Color::Yellow : sf::Color::White); btnMode4.setString(gameMode == 4 ? L"> 4. Reflex <" : L"4. Reflex");

    // fizyczne wypisywanie literek do okna
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

void UIManager::drawHUD(sf::RenderWindow& window, float elapsedTime, float timeLimit, int lives, float currentTrackTime, float trackingTimeRequired, float ddaMultiplier, StatsManager& stats) {
    if (!hasFont) return; // zabezpieczenie przed padnieciem gry
    string timeString = "Czas: " + to_string(static_cast<int>(elapsedTime)) + "s";
    if (timeLimit > 0.0f) timeString += " / " + to_string(static_cast<int>(timeLimit)) + "s";

    // rysowanie ikonek serduszek dla zyc
    if (gameMode == 2) {
        uiText.setString(sf::String(L"Czas: ") + to_string(static_cast<int>(elapsedTime)) + L"s | Życia: ");
        uiText.setPosition(20.f, 20.f); window.draw(uiText);
        float textWidth = uiText.getGlobalBounds().width;
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

void UIManager::drawReflexInstruct(sf::RenderWindow& window, int reflexState, float lastReactionTime) {
    sf::Text reflexInstruct;
    reflexInstruct.setFont(font); reflexInstruct.setCharacterSize(42); reflexInstruct.setFillColor(sf::Color::White);

    // tu sprawdzamy czy gracz zasnął na spacji w trybie na refleks
    if (reflexState == 0) {
        if (lastReactionTime < 0.0f) reflexInstruct.setString(L"FAŁSZYWY START! Naciśnij lewy przycisk, aby spróbować ponownie.");
        else if (lastReactionTime > 0.0f) reflexInstruct.setString(sf::String(L"Wynik: ") + to_string(static_cast<int>(lastReactionTime * 1000.f)) + L" ms. Kliknij, by zagrać znowu.");
        else reflexInstruct.setString(L"Naciśnij lewy przycisk myszy, aby rozpocząć.");
    }
    else if (reflexState == 1) reflexInstruct.setString(L"Czekaj na zielony...");
    else if (reflexState == 2) reflexInstruct.setString(L"KLIKNIJ!");

    reflexInstruct.setPosition(window.getSize().x / 2.0f - reflexInstruct.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f);
    window.draw(reflexInstruct);
}

void UIManager::drawGameOver(sf::RenderWindow& window, float elapsedTime, StatsManager& stats, int lives) {
    // wyswietla wlodarza FAME jak stracisz serduszka
    if (hasNajmanTex && gameMode == 2 && lives <= 0) window.draw(najmanSprite);

    // zbieranie tekstu o naszym wyniku przed wypisaniem na ekran
    sf::String summary = sf::String(L"GRACZ: ") + playerName + sf::String(L"\n-------------------------------\n");
    summary += sf::String(L"Czas trwania sesji: ") + to_string(static_cast<int>(elapsedTime)) + sf::String(L" sekund\n");

    if (gameMode == 4) {
        summary += sf::String(L"Tryb: Raw Reflex\n");
        summary += sf::String(L"Udane próby: ") + to_string(stats.getHits()) + sf::String(L"\n");
        summary += sf::String(L"Fałszywe starty: ") + to_string(stats.getMisses()) + sf::String(L"\n");
        summary += sf::String(L"Średni czas reakcji: ") + to_string(static_cast<int>(stats.getAverageReactionTime() * 1000.f)) + sf::String(L" ms\n");
    }
    else if (gameMode == 3) {
        float trackTime = stats.getTrackingTime(); float offTime = stats.getOffTargetTime();
        float totalTime = trackTime + offTime; float trackingAccuracy = (totalTime > 0) ? (trackTime / totalTime) * 100.0f : 0.0f;
        summary += sf::String(L"Tryb: Tracking\nZniszczone tarcze: ") + to_string(stats.getHits()) + sf::String(L"\nCzas na tarczy: ") + to_string(trackTime).substr(0,4) + sf::String(L"s\nCzas poza tarczą: ") + to_string(offTime).substr(0,4) + sf::String(L"s\nSkuteczność śledzenia: ") + to_string(trackingAccuracy).substr(0,5) + sf::String(L"%\n");
    }
    else if (gameMode == 2) {
        summary += sf::String(L"Tryb: Survival\nZniszczone cele: ") + to_string(stats.getHits()) + sf::String(L"\nŚredni czas reakcji: ") + to_string(stats.getAverageReactionTime()).substr(0,4) + sf::String(L"s\n");
        if (!stats.getDeathReason().empty()) summary += sf::String(L"Powód końca gry: ") + sf::String(stats.getDeathReason()) + sf::String(L"\n");
    } else {
        summary += sf::String(L"Tryb: Flicking\nTrafienia: ") + to_string(stats.getHits()) + sf::String(L" | Pudła: ") + to_string(stats.getMisses()) + sf::String(L"\nCelność ogólna: ") + to_string(static_cast<int>(stats.getAccuracy())) + sf::String(L"%\nŚredni czas reakcji: ") + to_string(stats.getAverageReactionTime()).substr(0,4) + sf::String(L"s\n");
    }

    statsSummaryText.setString(summary);
    window.draw(gameOverText); window.draw(statsSummaryText); window.draw(btnSaveYes); window.draw(btnSaveNo); window.draw(btnBackToMenu);
}

void UIManager::playNajmanSound() {
    if (hasNajmanSound) najmanSound.play();
}

// przywracamy kolory po restarcie
void UIManager::resetSaveUI() {
    btnSaveYes.setString(L"[ TAK - Zapisz wynik w pliku ]");
    btnSaveYes.setFillColor(sf::Color::Green);
    btnSaveNo.setString(L"[ NIE - Pomiń zapis ]");
    btnSaveNo.setFillColor(sf::Color::Red);
}

