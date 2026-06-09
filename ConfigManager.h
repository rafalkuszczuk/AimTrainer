#pragma once
#include <string>

using namespace std;

class ConfigManager {
private:
    float baseRadius;
    int targetCount;
    bool ddaEnabled;

public:
    ConfigManager();
    bool loadFromFile(const string& filename);

    float getBaseRadius() const;
    int getTargetCount() const;
    bool isDdaEnabled() const;
};
