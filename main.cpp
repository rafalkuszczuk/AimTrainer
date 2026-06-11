#include "GameEngine.h"

int main() {
    // Tworzymy główny silnik naszej gry - to on pod maską pociąga za wszystkie sznurki.
    GameEngine engine;

    // Odpalamy grę! W tym miejscu program wchodzi w główną pętlę (pętlę gry)
    // i kręci się w niej, dopóki gracz nie wyłączy okna.
    engine.run();

    return 0; // Wszystko poszło gładko, zgłaszamy systemowi Windows/Linux pełen sukces (0).
}
