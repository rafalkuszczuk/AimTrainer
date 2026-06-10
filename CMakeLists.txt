cmake_minimum_required(VERSION 3.5)
project(AimTrainer LANGUAGES CXX)

# Wymuszenie standardu C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Twarde wskazanie ścieżki do folderu SFML na Twoim dysku (PAMIĘTAJ O UKOŚNIKACH '/')
set(CMAKE_PREFIX_PATH "C:/Qt/SFML")
# Szukanie biblioteki SFML w wersji 2.x (wymagane moduły grafiki, okna i systemu)
find_package(SFML 2 COMPONENTS graphics window system audio REQUIRED)

# Tutaj dodajecie WSZYSTKIE pliki .cpp, które powstaną w projekcie (z czasem ta lista będzie rosnąć)
add_executable(AimTrainer
    main.cpp
    MovingTarget.h MovingTarget.cpp
    Target.h
    StaticTarget.h StaticTarget.cpp
    GameEngine.h GameEngine.cpp
    ConfigManager.h ConfigManager.cpp
    StatsManager.h StatsManager.cpp
    InputManager.h InputManager.cpp
)

# Podłączenie znalezionych modułów SFML do naszego programu
target_link_libraries(AimTrainer PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
# --- ZAAWANSOWANE BEZPIECZNE KOPIOWANIE ASSETÓW ---
# Tworzymy listę wszystkich plików, których nasza gra MOŻE (ale nie musi) używać
set(ASSET_FILES
    arial.ttf
    crosshair.png
    target.png
    shoot.wav
    hit.wav
    najman.png
    najman.wav
    dead.wav
)

# Pętla: Dla każdego pliku z listy sprawdzamy, czy fizycznie istnieje na dysku
foreach(FILE IN LISTS ASSET_FILES)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/${FILE}")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/${FILE} ${CMAKE_BINARY_DIR}/${FILE} COPYONLY)
    endif()
endforeach()
