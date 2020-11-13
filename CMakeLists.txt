cmake_minimum_required(VERSION 3.18)

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSROOT /opt/arm-linux-gnuabi-sysroot)
SET(LINKER_PATH /opt/local/bin/arm-none-linux-gnueabi-ld)
# /opt/clang+llvm-11.0.0-x86_64-apple-darwin/bin/ld.lld

set(CMAKE_C_COMPILER clang-mp-10)
set(CMAKE_CXX_COMPILER clang++-mp-10)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=${LINKER_PATH}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_TARGET arm-linux-gnueabihf)
set(CMAKE_CXX_COMPILER_TARGET arm-linux-gnueabihf)

set(CMAKE_C_STANDARD 11)
add_definitions(-DDEBUG)
add_compile_options(-g -Og -Wall -Wextra)

project(hello-raspberry)

file(GLOB SOURCES src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})
