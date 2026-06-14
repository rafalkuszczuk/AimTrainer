/**
 * @file StatsManager.h
 * @author Sebastian Mielcarek
 * @brief Logika UI, implementacja algorytmu DDA, zapis analityki do plików (I/O) oraz wczytywanie zasobów.
 */
#pragma once
#include <vector>
#include <string>

// klasa zbiera wszystkie dane z gry zeby potem je pokazac
class StatsManager {
private:
    int hits = 0;   // ile razy trafilismy
    int misses = 0; // ile razy strzelilismy w sciane
    std::vector<float> reactionTimes; // lista wszystkich czasow reakcji

    float totalTrackingTime = 0.0f; // ile czasu lacznie trzymalismy celownik na tarczy
    float timeOffTarget = 0.0f;     // ile czasu celownik uciekal w tlo

    // czemu zginelismy w trybie survival
    std::string deathReason = "";

public:
    void addHit(float reactionTime = 0.0f);
    void addMiss();

    // dorzucanie czasu do sledzenia
    void addTrackingTime(float dt) { totalTrackingTime += dt; }
    void addOffTargetTime(float dt) { timeOffTarget += dt; }

    // ustawianie i pobieranie powodu smierci
    void setDeathReason(const std::string& reason) { deathReason = reason; }
    std::string getDeathReason() const { return deathReason; }

    // narzedzia do wyciagania wynikow
    float getAccuracy() const;
    int getHits() const { return hits; }
    int getMisses()  { return misses; }
    float getTrackingTime() const { return totalTrackingTime; }
    float getOffTargetTime() const { return timeOffTarget; }
    float getAverageReactionTime() const; // liczy srednia z listy

    // zapis do pliku albo wypisanie w czarnym oknie
    void saveReport(const std::string& filename, const std::string& playerName, int mode) const;
    void printReportConsole(int mode) const;
};
