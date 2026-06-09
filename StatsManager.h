#pragma once
#include <string>
#include <vector>

using namespace std;

class StatsManager {
private:
    int hits;
    int misses;
    vector<float> reactionTimes;

public:
    StatsManager();

    void addHit(float reactionTime);
    void addMiss();

    float getAccuracy() const;
    float getAverageReactionTime() const;

    bool saveReport(const string& filename) const;
};
