cmake_minimum_required(VERSION 3.29)

project(RFC4648 C)

set(CMAKE_C_STANDARD 17)

enable_testing()

add_subdirectory("src")
add_subdirectory("tests")
add_subdirectory("examples")