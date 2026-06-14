# Aim Trainer - zaawansowany symulator celności (C++ & SFML)

Projekt zaliczeniowy z przedmiotu Programowanie Strukturalne i Obiektowe. Podczas planowania architektury celowo zrezygnowaliśmy z tworzenia standardowych gier zręcznościowych (typu Space Shooter czy Tower Defense). Zauważyliśmy, że w tego typu projektach praca studenta często rozmywa się na poboczne aspekty, takie jak tworzenie animacji czy projektowanie map. Zamiast tego postawiliśmy na stworzenie zaawansowanego narzędzia e-sportowego, w którym na pierwszym miejscu stoi inżynieria, optymalizacja i precyzja.
Gra testuje i poprawia czas reakcji, precyzję oraz zdolność śledzenia celów przez gracza, oferując 4 unikalne tryby rozgrywki oraz system Dynamicznego Dostosowywania Poziomu Trudności (DDA).

##  Zespół i Podział Obowiązków

**Rafał Kuszczuk** pełnił rolę głównego integratora repozytorium. Odpowiadał za łączenie modułów pisanych przez cały zespół, pilnowanie działania CMake i wypychanie zintegrowanych wersji na serwer (stąd jego przeważająca obecność w historii commitów). 

Właściwa praca i pisanie kodu zostały podzielone na równe domeny:
* **Filip Krzyżkowiak:** Architektura silnika (`GameEngine`), inicjalizacja okna, główna pętla gry, zarządzanie pamięcią (wektor wskaźników) oraz polimorficzna logika fabryki celów. Stworzone pliki: main.cpp, GameEngine.h, GameEngine.cpp, Target.h
* **Rafał Kuszczuk (Integrator):** Zaawansowana interakcja, fizyka, implementacja "raw input" myszy, matematyka detekcji kolizji (hitboxy) oraz wektory ruchu dla klas `Target`. Stworzone pliki: InputManager.h, InputManager.cpp, MovingTarget.h, MovingTarget.cpp, StaticTarget.h, StaticTarget.cpp
* **Sebastian Mielcarek:** Logika gry, architektura klas zarządzających (UI/Stats), implementacja algorytmu DDA, zapis analityki do plików oraz wczytywanie zasobów. Stworzone pliki: StatsManager.h, StatsManager.cpp, UIManager.h, UIManager.cpp

---

##  Realizacja wymagań projektowych

Projekt spełnia (a w wielu miejscach przewyższa) wymagania określone w wymaganiach projektu:

- [x] **Podział na pliki .cpp i .h:** Cały projekt został zhermetyzowany. Każda klasa (silnik, menedżery, obiekty gry) posiada osobny plik nagłówkowy oraz plik z definicjami.
- [x] **Zastosowanie dziedziczenia:** Klasy `StaticTarget` oraz `MovingTarget` dziedziczą po abstrakcyjnej klasie bazowej `Target`.
- [x] **Zastosowanie polimorfizmu:** Metody `update()` oraz `draw()` są funkcjami wirtualnymi (virtual) w klasie bazowej `Target` i są nadpisywane (override) w klasach pochodnych.
- [x] **Wszystkie obiekty w jednym kontenerze:** Użyto kontenera `std::vector` przechowującego polimorficzne obiekty: `std::vector<std::unique_ptr<Target>> targets;`.
- [x] **Wykorzystanie wskaźników (smart pointers):** Ręczne zarządzanie pamięcią zastąpiono bezpiecznymi wskaźnikami `std::unique_ptr` oraz funkcją `std::make_unique`, chroniąc projekt przed wyciekami pamięci.
- [x] **Podział na public/protected/private:** Ścisła hermetyzacja danych. W klasie bazowej `Target` użyto modyfikatora `protected`, aby klasy pochodne mogły modyfikować wspólne parametry (x, y, radius).
- [x] **Komentarze:** W ostatecznej wersji kodu źródłowego programu zawarto komentarze opisujące logikę, podział prac, poszczególne klasy oraz kluczowe metody, co ułatwia zrozumienie działania modułów.
- [x] **Animacje:** Dynamiczne, płynne kurczenie się tarcz po trafieniu, oparte o czas klatki.
- [x] **Ruch bazujący na czasie i prędkości liniowej:** Klasa `MovingTarget` operuje wektorami `velocityX` i `velocityY`, które są mnożone przez parametr `deltaTime` (czas między klatkami). Zapewnia to identyczną fizykę odbijania się od ścian na komputerach o różnej liczbie FPS.
- [x] **Zapis i odczyt plików:** Klasa `StatsManager` realizuje zapisywanie szczegółowych raportów, statystyk (trafienia, pudła, skuteczność, czasy reakcji) do pliku tekstowego `wyniki.txt` za pomocą biblioteki `<fstream>`.
- [x] **Wprowadzenie losowości:** Wykorzystano zaawansowany generator liczb pseudolosowych `<random>` (`std::mt19937`) do losowania wielkości tarcz, pozycji na ekranie, czasu pojawiania się oraz wektorów wejściowych prędkości tarcz ruchomych.
- [x] **Parametryzacja gry:** Program pozwala na zmianę głównych zmiennych za pomocą rozbudowanego interfejsu (zmiana czasu gry, trybu, wielkości celów, kolorystyki tła/celownika oraz włączanie algorytmu).
- [x] **Kreatywne rozwiązania programistyczne (Punkty Dodatkowe):**
  1. **Algorytm DDA (Dynamic Difficulty Adjustment):** System bada celność gracza w czasie rzeczywistym i przy użyciu mnożnika `ddaMultiplier` modyfikuje rozmiar oraz czas "życia" tarcz bez konieczności resetowania poziomu.
  2. **Raw Input (Klasa InputManager):** Całkowite odseparowanie fizycznego kursora systemu operacyjnego od wirtualnego celownika w grze. Obliczanie przesunięcia za pomocą delty pikseli względem środka ekranu.
  3. **Wydajna Detekcja Kolizji:** Zastąpienie kosztownych obliczeniowo pierwiastków (sqrt) na korzyść kwadratów odległości z twierdzenia Pitagorasa.

---

##  Organizacja repozytorium GitHub 
Wszystkie pliki binarne (`.exe`, dll, pliki kompilacji) zostały poprawnie wykluczone z repozytorium (użyto pliku `.gitignore`).

---

##  Oświadczenie o plikach 3rd party (Media)
Zgodnie z wymaganiami projektu, poniżej znajduje się lista użytych zasobów zewnętrznych. Wszystkie zasoby zostały dodane wyłącznie jako pliki statyczne w folderze `assets/` i są automatycznie ładowane (choć ich brak nie wpływa na możliwość poprawnej kompilacji programu!) w kodzie źródłowym za pomocą standardowych mechanizmów SFML.

* Czcionka `arial.ttf` - 3rd party: Microsoft Corporation (domyślna czcionka systemowa udostępniana do celów edukacyjnych).
* Tekstury (`crosshair.png`, `najman.png`) - 3rd party: Zmodyfikowane grafiki typu freeware zebrane z serwisów do udostępniania zasobów graficznych PNG (m.in. opengameart.org).
* Pliki dźwiękowe (`shoot.wav`, `hit.wav`, `dead.wav`, `najman.wav`) - 3rd party: Darmowe paczki sampli SFX oraz fragmenty wycięte na potrzeby edukacyjne w programie Audacity.
