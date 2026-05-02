cmake_minimum_required(VERSION 3.14)
project(KruskalMST)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0)
FetchContent_MakeAvailable(googletest)

add_executable(main src/main.c src/kruskal.c)
target_include_directories(main PRIVATE src)

add_executable(my_test src/tests.cpp src/kruskal.c)
target_include_directories(my_test PRIVATE src)
target_link_libraries(my_test gtest gtest_main)