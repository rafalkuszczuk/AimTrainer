#include "ConfigManager.h"
#include <fstream>
#include <iostream>

using namespace std;

ConfigManager::ConfigManager() : baseRadius(25.0f), targetCount(3), ddaEnabled(false) {}

bool ConfigManager::loadFromFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "[ConfigManager] Blad: Nie mozna otworzyc pliku " << filename << ". Uzywam wartosci domyslnych." << endl;
        return false;
    }

    string key;
    while (file >> key) {
        if (key == "baseRadius") {
            file >> baseRadius;
        } else if (key == "targetCount") {
            file >> targetCount;
        } else if (key == "ddaEnabled") {
            file >> ddaEnabled;
        }
    }

    file.close();
    cout << "[ConfigManager] Konfiguracja wczytana pomyslnie." << endl;
    return true;
}

float ConfigManager::getBaseRadius() const { return baseRadius; }
int ConfigManager::getTargetCount() const { return targetCount; }
bool ConfigManager::isDdaEnabled() const { return ddaEnabled; }
