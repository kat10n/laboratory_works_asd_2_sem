cmake_minimum_required(VERSION 4.0)
project(laboratory_works_asd_2_sem)

set(CMAKE_CXX_STANDARD 14)

include_directories(3_lab_calc/src)

add_executable(laboratory_works_asd_2_sem
        3_lab_calc/src/enter.c
        3_lab_calc/src/main.c
        3_lab_calc/src/mini_calculator.c
        3_lab_calc/src/reverse_polish_notation.c
        3_lab_calc/src/stack.c
        3_lab_calc/src/struct.h
        3_lab_calc/src/tokenize.c
        3_lab_calc/tests/tests.cpp
        3_lab_calc/src/enter.h
        3_lab_calc/src/tokenize.h
        3_lab_calc/src/reverse_polish_notation.h
        3_lab_calc/src/mini_calculator.h
        3_lab_calc/src/stack.h)
