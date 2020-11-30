cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

project(hello-linux)

add_definitions(-DDEBUG)
add_compile_options(-ggdb3 -Og -Wall -Wextra)

# Build 32 bits
add_compile_options(-m32)
add_link_options(-m32)

file(GLOB SOURCES src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})
