#include "StatsManager.h"
#include <fstream>
#include <iostream>
#include <numeric>

using namespace std;

StatsManager::StatsManager() : hits(0), misses(0) {}

void StatsManager::addHit(float reactionTime) {
    hits++;
    reactionTimes.push_back(reactionTime);
}

void StatsManager::addMiss() {
    misses++;
}

float StatsManager::getAccuracy() const {
    int totalShots = hits + misses;
    if (totalShots == 0) return 0.0f;

    return (static_cast<float>(hits) / totalShots) * 100.0f;
}

float StatsManager::getAverageReactionTime() const {
    if (reactionTimes.empty()) return 0.0f;

    float sum = accumulate(reactionTimes.begin(), reactionTimes.end(), 0.0f);
    return sum / reactionTimes.size();
}

bool StatsManager::saveReport(const string& filename) const {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "[StatsManager] Blad: Nie mozna zapisac raportu do " << filename << endl;
        return false;
    }

    file << "=== RAPORT AIMTRAINER ===\n";
    file << "Trafienia: " << hits << "\n";
    file << "Pudla: " << misses << "\n";
    file << "Celność: " << getAccuracy() << "%\n";
    file << "Sredni czas reakcji: " << getAverageReactionTime() << " s\n";
    file << "=========================\n";

    file.close();
    cout << "[StatsManager] Raport analityczny zapisany jako: " << filename << endl;
    return true;
}
