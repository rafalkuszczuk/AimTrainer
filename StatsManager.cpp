#include "StatsManager.h"
#include <iostream>
#include <numeric>
#include <fstream>

using namespace std;

void StatsManager::addHit(float reactionTime) {
    hits++; // +1 do trafień. Dobry chłopak!

    // Nie każdy tryb zapisuje czasy reakcji. Zapisujemy tylko kiedy wynik jest większy niż 0.
    if (reactionTime > 0.0f) {
        reactionTimes.push_back(reactionTime); // "push_back" to wrzucenie wyniku na koniec wektora (listy).
    }
}

void StatsManager::addMiss() {
    misses++; // Niestety, celownik minął się z celem.
}

float StatsManager::getAccuracy() const {
    // Zabezpieczenie przed błędem Dzielenia Przez Zero!
    // Jeżeli gracz nawet nie odpalił broni, oddajemy mu po prostu równe 0%.
    if (hits + misses == 0) return 0.0f;

    // Zwykła matematyka: ilość trafień / wszystkie strzały * 100%.
    // static_cast<float> wymusza ułamki, inaczej "1 trafienie na 2 strzały" dałoby 0 zamiast 0.5.
    return (static_cast<float>(hits) / (hits + misses)) * 100.0f;
}

float StatsManager::getAverageReactionTime() const {
    if (reactionTimes.empty()) return 0.0f; // Brak danych = wynik 0

    // Bierze listę reakcji (od pierwszej do ostatniej) i dodaje każdą liczbę do siebie, startując od "0.0f".
    // Na koniec dzieli całą, ogromną sumę przez ilość tych wyników (size). To wzór na średnią arytmetyczną!
    return accumulate(reactionTimes.begin(), reactionTimes.end(), 0.0f) / reactionTimes.size();
}

void StatsManager::saveReport(const string& filename, const string& playerName, int mode) const {
    // Działa to jak notatnik! "app" (append) znaczy DOPISZ DO KOŃCA.
    // Gdyby nie to "app", komputer wymazałby cały plik gumką i zapisał w nim tylko TEN, najnowszy wynik.
    ofstream file(filename, ios::app);

    // Sprawdzamy czy system operacyjny łaskawie pozwolił otworzyć plik
    if (file.is_open()) {
        file << "[" << playerName << "] | Tryb: "; // Ładne rozpoczęcie z Nickiem gracza.

        if (mode == 3) {
            // Zwykłe policzenie procencików dla trybu Tracking.
            float totalTime = totalTrackingTime + timeOffTarget;
            // Zapis (totalTime > 0) ? TO : TAMTO - "czy czas większy od 0? Jak tak to pierwsze obliczenie, jak nie to drugie"
            float trackingAccuracy = (totalTime > 0) ? (totalTrackingTime / totalTime) * 100.0f : 0.0f;
            file << "Tracking | Celów: " << hits
                 << " | Skuteczność śledzenia: " << trackingAccuracy << "%" << endl;

        } else if (mode == 2) {
            file << "Survival | Trafienia: " << hits
                 << " | Śr. reakcja: " << getAverageReactionTime() << "s"
                 // Tutaj ten sam trik: Sprawdź czy pole deathReason jest puste.
                 // Jak puste (true) napisz "Wyjście", w przeciwnym razie napisz podany string.
                 << " | Powód końca: " << (deathReason.empty() ? "Wyjście" : deathReason) << endl;

        } else {
            file << "Reflex   | Trafienia: " << hits << " | Pudła: " << misses
                 << " | Celność: " << getAccuracy() << "%"
                 << " | Śr. reakcja: " << getAverageReactionTime() << "s" << endl;
        }
        file.close(); // Ważne! Zamknięcie pliku uwalnia go, inaczej inne programy by do niego nie miały dostępu.
    }
}

void StatsManager::printReportConsole(int mode) const {
    // Dokładnie to samo, co wyżej w pliku tekstowym, ale rysuje te napisy w oknie konsoli CMD po meczu.
    cout << "\n=== WYNIKI TWOJEJ SESJI ===" << endl;

    if (mode == 3) {
        // Raport Tracking
        cout << "Zniszczone tarcze: " << hits << endl;
        cout << "Czas celownika NA tarczy: " << totalTrackingTime << " s" << endl;
        cout << "Czas celownika POZA tarcza: " << timeOffTarget << " s" << endl;

        float totalTime = totalTrackingTime + timeOffTarget;
        float trackingAccuracy = (totalTime > 0) ? (totalTrackingTime / totalTime) * 100.0f : 0.0f;
        cout << "Ogolna skutecznosc sledzenia: " << trackingAccuracy << "%" << endl;

    } else if (mode == 2) {
        // Raport Survival (Z przyczyną śmierci)
        cout << "Trafienia: " << hits << endl;
        cout << "Sredni czas reakcji: " << getAverageReactionTime() << " s" << endl;

        // "Jeśli powód śmierci NIE jest (!) pusty..."
        if (!deathReason.empty()) {
            cout << ">>> PRZYCZYNA KONCA GRY: " << deathReason << " <<<" << endl;
        } else {
            cout << ">>> PRZYCZYNA KONCA GRY: Koniec czasu lub wyjscie (ESC) <<<" << endl;
        }

    } else {
        // Raport Reflex
        cout << "Trafienia: " << hits << endl;
        cout << "Pudla: " << misses << endl;
        cout << "Celnosc ogolna: " << getAccuracy() << "%" << endl;
        cout << "Sredni czas reakcji: " << getAverageReactionTime() << " s" << endl;
    }
    cout << "===========================\n" << endl; // Odgradzacz kosmetyczny na sam dół.
}
