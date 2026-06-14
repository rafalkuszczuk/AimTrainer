/**
 * @file StatsManager.cpp
 * @author Sebastian Mielcarek
 * @brief Logika UI, implementacja algorytmu DDA, zapis analityki do plików (I/O) oraz wczytywanie zasobów.
 */
#include "StatsManager.h"
#include <iostream>
#include <numeric>
#include <fstream>

using namespace std;

void StatsManager::addHit(float reactionTime) {
    hits++; // dodajemy punkt

    // jak podalismy czas to dopisujemy go do listy
    if (reactionTime > 0.0f) {
        reactionTimes.push_back(reactionTime);
    }
}

void StatsManager::addMiss() {
    misses++; // gracz spudlowal
}

float StatsManager::getAccuracy() const {
    // zeby nie dzielic przez zero jak ktos nie strzela
    if (hits + misses == 0) return 0.0f;

    // prosta matma na procenty z trafien
    return (static_cast<float>(hits) / (hits + misses)) * 100.0f;
}

float StatsManager::getAverageReactionTime() const {
    // brak danych
    if (reactionTimes.empty()) return 0.0f;

    // dodaje wszystkie czasy do siebie i dzieli przez ich ilosc zeby miec srednia
    return accumulate(reactionTimes.begin(), reactionTimes.end(), 0.0f) / reactionTimes.size();
}

void StatsManager::saveReport(const string& filename, const string& playerName, int mode) const {
    // otwieramy plik, app oznacza ze dopisujemy na koniec a nie kasujemy calosci
    ofstream file(filename, ios::app);

    if (file.is_open()) {
        file << "[" << playerName << "] | Tryb: ";

        // rozne teksty w zaleznosci od wybranego trybu
        if (mode == 3) {
            float totalTime = totalTrackingTime + timeOffTarget;
            float trackingAccuracy = (totalTime > 0) ? (totalTrackingTime / totalTime) * 100.0f : 0.0f;
            file << "Tracking | Celów: " << hits
                 << " | Skuteczność śledzenia: " << trackingAccuracy << "%" << endl;

        } else if (mode == 2) {
            file << "Survival | Trafienia: " << hits
                 << " | Śr. reakcja: " << getAverageReactionTime() << "s"
                 // jak pusty powod to piszemy wyjscie
                 << " | Powód końca: " << (deathReason.empty() ? "Wyjście" : deathReason) << endl;

        } else {
            file << "Reflex   | Trafienia: " << hits << " | Pudła: " << misses
                 << " | Celność: " << getAccuracy() << "%"
                 << " | Śr. reakcja: " << getAverageReactionTime() << "s" << endl;
        }
        file.close(); // zamykamy plik zeby inni mogli go uzyc
    }
}

// ta funkcja robi to samo co wyzej tylko pisze w czarnym oknie konsoli
void StatsManager::printReportConsole(int mode) const {
    cout << "\n=== WYNIKI TWOJEJ SESJI ===" << endl;

    if (mode == 3) {
        cout << "Zniszczone tarcze: " << hits << endl;
        cout << "Czas celownika NA tarczy: " << totalTrackingTime << " s" << endl;
        cout << "Czas celownika POZA tarcza: " << timeOffTarget << " s" << endl;

        float totalTime = totalTrackingTime + timeOffTarget;
        float trackingAccuracy = (totalTime > 0) ? (totalTrackingTime / totalTime) * 100.0f : 0.0f;
        cout << "Ogolna skutecznosc sledzenia: " << trackingAccuracy << "%" << endl;

    } else if (mode == 2) {
        cout << "Trafienia: " << hits << endl;
        cout << "Sredni czas reakcji: " << getAverageReactionTime() << " s" << endl;

        if (!deathReason.empty()) {
            cout << ">>> PRZYCZYNA KONCA GRY: " << deathReason << " <<<" << endl;
        } else {
            cout << ">>> PRZYCZYNA KONCA GRY: Koniec czasu lub wyjscie (ESC) <<<" << endl;
        }

    } else {
        cout << "Trafienia: " << hits << endl;
        cout << "Pudla: " << misses << endl;
        cout << "Celnosc ogolna: " << getAccuracy() << "%" << endl;
        cout << "Sredni czas reakcji: " << getAverageReactionTime() << " s" << endl;
    }
    cout << "===========================\n" << endl;
}
