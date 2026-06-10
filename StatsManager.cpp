#include "StatsManager.h"
#include <iostream>
#include <numeric>
#include <fstream>

using namespace std;

void StatsManager::addHit(float reactionTime) {
    hits++;
    if (reactionTime > 0.0f) {
        reactionTimes.push_back(reactionTime);
    }
}

void StatsManager::addMiss() {
    misses++;
}

float StatsManager::getAccuracy() const {
    if (hits + misses == 0) return 0.0f;
    return (static_cast<float>(hits) / (hits + misses)) * 100.0f;
}

float StatsManager::getAverageReactionTime() const {
    if (reactionTimes.empty()) return 0.0f;
    return accumulate(reactionTimes.begin(), reactionTimes.end(), 0.0f) / reactionTimes.size();
}

void StatsManager::saveReport(const string& filename) const {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << "Zapisano wyniki sesji." << endl; // Tutaj Sebastian może rozbudować logikę zapisu do pliku
        file.close();
    }
}

void StatsManager::printReportConsole(int mode) const {
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
    cout << "===========================\n" << endl;
}

