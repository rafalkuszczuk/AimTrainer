cmake_minimum_required(VERSION 3.5)
project(AimTrainerPro LANGUAGES CXX)

# Wymuszenie standardu C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AimTrainerPro main.cpp)
# Poniżej w przyszłości dodacie komendy linkujące bibliotekę SFML 2.x


add_executable(AimTrainer main.cpp)

include(GNUInstallDirs)
install(TARGETS AimTrainer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
