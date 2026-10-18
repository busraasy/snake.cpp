cmake_minimum_required(VERSION 3.16)
project(SnakeGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML einbinden
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Quellcodes hinzufügen
add_executable(SnakeGame src/main.cpp)

# SFML-Bibliotheken verlinken
target_link_libraries(SnakeGame PRIVATE sfml-graphics sfml-window sfml-system)