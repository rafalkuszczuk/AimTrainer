#pragma once
#include <vector>
#include <string>

class StatsManager {
private:
    int hits = 0;
    int misses = 0;
    std::vector<float> reactionTimes;

    float totalTrackingTime = 0.0f;
    float timeOffTarget = 0.0f;

    // Nowa zmienna dla trybu Survival
    std::string deathReason = "";

public:
    void addHit(float reactionTime = 0.0f);
    void addMiss();

    void addTrackingTime(float dt) { totalTrackingTime += dt; }
    void addOffTargetTime(float dt) { timeOffTarget += dt; }

    // Metody do obsługi przyczyny końca gry
    void setDeathReason(const std::string& reason) { deathReason = reason; }
    std::string getDeathReason() const { return deathReason; }

    float getAccuracy() const;
    float getAverageReactionTime() const;

    void saveReport(const std::string& filename, const std::string& playerName, int mode) const;
    void printReportConsole(int mode) const;
};
