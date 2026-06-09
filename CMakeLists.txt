cmake_minimum_required(VERSION 3.5)
project(AimTrainer LANGUAGES CXX)

# Wymuszenie standardu C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Szukanie biblioteki SFML w wersji 2.x (wymagane moduły grafiki, okna i systemu)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

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
target_link_libraries(AimTrainer PRIVATE sfml-graphics sfml-window sfml-system)
