cmake_minimum_required(VERSION 3.5)
# Nazwa naszego projektu i używany język (C++)
project(AimTrainer LANGUAGES CXX)

# Wymuszenie standardu C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wskazujemy kompilatorowi, gdzie na dysku leży SFML
# Twarde wskazanie ścieżki do folderu SFML na Twoim dysku (PAMIĘTAJ O UKOŚNIKACH '/')
set(CMAKE_PREFIX_PATH "C:/Qt/SFML")

# Szukanie biblioteki SFML w wersji 2.x (wymagane moduły grafiki, okna, systemu i audio)
find_package(SFML 2 COMPONENTS graphics window system audio REQUIRED)

# Tutaj dodajemy WSZYSTKIE pliki .cpp i .h naszego projektu, zeby kompilator wiedzial co budowac
add_executable(AimTrainer
    main.cpp
    MovingTarget.h MovingTarget.cpp
    Target.h
    StaticTarget.h StaticTarget.cpp
    GameEngine.h GameEngine.cpp
    StatsManager.h StatsManager.cpp
    InputManager.h InputManager.cpp
)

# Podłączenie znalezionych modułów SFML do naszego programu (żeby okienko i dźwięki w ogóle działały)
target_link_libraries(AimTrainer PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# --- SKRYPT DO AUTOMATYCZNEGO KOPIOWANIA PLIKÓW (ASSETÓW) ---
# Tworzymy listę wszystkich plików (czcionki, obrazki, dźwięki), których gra używa
set(ASSET_FILES
    arial.ttf
    target.png
    shoot.wav
    hit.wav
    najman.png
    najman.wav
    dead.wav
)

# Pętla: Dla każdego pliku z listy sprawdzamy, czy fizycznie istnieje na dysku.
# Jeśli tak, CMake sam skopiuje go do folderu z plikiem .exe, żebyśmy nie musieli tego robić ręcznie!
foreach(FILE IN LISTS ASSET_FILES)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/${FILE}")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/${FILE} ${CMAKE_BINARY_DIR}/${FILE} COPYONLY)
    endif()
endforeach()
