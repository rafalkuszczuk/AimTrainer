#pragma once
#include <vector>
#include <string>

// Klasa "Księgowy". Zbiera wszystkie dane: trafienia, pudełka, czasy reakcji.
class StatsManager {
private:
    int hits = 0;   // Ile trafień?
    int misses = 0; // Ile strzałów poszło w próżnię?
    std::vector<float> reactionTimes; // Tablica (lista) przechowująca każdy czas reakcji po kolei

    float totalTrackingTime = 0.0f; // Ile czasu (łącznie) gracz trzymał kursor NA obiekcie
    float timeOffTarget = 0.0f;     // Ile czasu uciekało mu gdzieś w tle

    // Nowa zmienna dla trybu Survival - jak "zmarliśmy"
    std::string deathReason = "";

public:
    void addHit(float reactionTime = 0.0f);
    void addMiss();

    // Dodawanie czasu śledzenia
    void addTrackingTime(float dt) { totalTrackingTime += dt; }
    void addOffTargetTime(float dt) { timeOffTarget += dt; }

    // Metody do obsługi przyczyny końca gry w survivalu
    void setDeathReason(const std::string& reason) { deathReason = reason; }
    std::string getDeathReason() const { return deathReason; }

    // Narzędzia wyciągające podliczenia
    float getAccuracy() const;
    int getHits() const { return hits; }
    int getMisses()  { return misses; }
    float getTrackingTime() const { return totalTrackingTime; }
    float getOffTargetTime() const { return timeOffTarget; }
    float getAverageReactionTime() const; // Średni czas reakcji z całej gry

    // Narzędzia zapisujące statystyki (konsola lub plik .txt na dysku)
    void saveReport(const std::string& filename, const std::string& playerName, int mode) const;
    void printReportConsole(int mode) const;
};
