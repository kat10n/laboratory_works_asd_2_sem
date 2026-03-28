cmake_minimum_required(VERSION 3.14)
project(calc C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

# ── Библиотека из всей логики (без main) ─────────────────────────────────────
set(LIB_SOURCES
        src/stack.c
        src/enter.c
        src/tokenize.c
        src/reverse_polish_notation.c
        src/mini_calculator.c
)

add_library(calc_lib ${LIB_SOURCES})
target_include_directories(calc_lib PUBLIC src)

# ── Основная программа ────────────────────────────────────────────────────────
add_executable(calc src/main.c)
target_link_libraries(calc PRIVATE calc_lib)

# ── Тесты ─────────────────────────────────────────────────────────────────────
add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE calc_lib gtest gtest_main)
