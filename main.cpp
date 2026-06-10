#include "GameEngine.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    int selectedMode = 1;
    string playerName;

    cout << "Podaj swoj Nick: ";
    cin >> playerName;

    while (true) {
        cout << "\n=== AIM TRAINER PRO ===" << endl;
        cout << "Witaj, " << playerName << "!" << endl;
        cout << "1. Tryb Reflex (Statyczne tarcze)" << endl;
        cout << "2. Tryb Survival (Serduszka, pulapki)" << endl;
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
            cout << "Podaj czas utrzymania celownika do zniszczenia tarczy (np. 1.5): ";
            cin >> trackingReq;
        }

        int targetSizeOpt = 4;
        cout << "Wybierz rozmiar tarcz (1-Male, 2-Srednie, 3-Duze, 4-Losowe): ";
        cin >> targetSizeOpt;

        // NOWOŚĆ: Pytanie o DDA (tylko jeśli to nie jest tryb Tracking)
        bool enableDDA = false;
        if (selectedMode != 3) {
            int ddaChoice;
            cout << "Czy wlaczyc system dynamicznej trudnosci DDA? (1 - Tak, 0 - Nie): ";
            cin >> ddaChoice;
            enableDDA = (ddaChoice == 1);
        }

        // Przekazujemy nową zmienną enableDDA na samym końcu
        GameEngine engine(playerName, selectedMode, trackingReq, timeLimit, targetSizeOpt, enableDDA);
        engine.run();
    }

    return 0;
}
