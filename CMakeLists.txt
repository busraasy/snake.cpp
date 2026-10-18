cmake_minimum_required(VERSION 3.16)
project(SnakeGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML einbinden
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
# Quellcodes hinzufügen
add_executable(SnakeGame src/main.cpp)

# SFML-Bibliotheken verlinken
target_link_libraries(SnakeGame PRIVATE SFML::Graphics SFML::Window SFML::System)