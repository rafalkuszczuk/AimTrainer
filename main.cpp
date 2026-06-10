#include "GameEngine.h"
#include <iostream>

using namespace std;

int main() {
    int selectedMode = 1;

    while (true) {
        cout << "\n=== AIM TRAINER PRO ===" << endl;
        cout << "1. Tryb Reflex (Statyczne tarcze)" << endl;
        cout << "2. Tryb Survival (Serduszka, ZIELONE pulapki - nie strzelaj!)" << endl;
        cout << "3. Tryb Tracking (Sledzenie 1 tarczy, odbijanie od scian)" << endl;
        cout << "0. Wyjscie z gry" << endl;
        cout << "Wybierz tryb (0-3): ";
        cin >> selectedMode;

        if (selectedMode == 0) {
            cout << "Zamykanie gry. Do zobaczenia!" << endl;
            break;
        }

        float timeLimit = 0.0f;
        cout << "Podaj limit czasu w sekundach (Wpisz 0, aby grac bez limitu - do wcisniecia ESC): ";
        cin >> timeLimit;

        float trackingReq = 0.0f;
        if (selectedMode == 3) {
            cout << "Podaj czas utrzymania celownika do zniszczenia tarczy (w sekundach, np. 1.5): ";
            cin >> trackingReq;
        }

        // Nowe menu wyboru wielkości tarcz
        int targetSizeOpt = 4;
        cout << "Wybierz rozmiar tarcz:" << endl;
        cout << "1. Male (Hardcore)" << endl;
        cout << "2. Srednie (Standard)" << endl;
        cout << "3. Duze (Rozgrzewka)" << endl;
        cout << "4. Losowe (Mieszane)" << endl;
        cout << "Wybierz opcje (1-4): ";
        cin >> targetSizeOpt;

        // Przekazujemy nową zmienną do silnika
        GameEngine engine(selectedMode, trackingReq, timeLimit, targetSizeOpt);
        engine.run();
    }

    return 0;
}
