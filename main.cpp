/**
 * @file main.cpp
 * @author Filip Krzyżkowiak
 * @brief Architektura silnika, inicjalizacja okna, główna pętla gry, zarządzanie pamięcią oraz polimorfizm obiektów.
 */
#include "GameEngine.h"

int main() {
    // odpalamy główny silnik naszej gry
    GameEngine engine;

    // gra sie kreci w kolko az nie zamkniemy okna
    engine.run();

    return 0; // wszystko poszlo dobrze
}
