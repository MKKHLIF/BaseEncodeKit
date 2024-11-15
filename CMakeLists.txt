cmake_minimum_required(VERSION 3.29)

project(RFC4648 C)

set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE SRC_FILES "*.c")
file(GLOB_RECURSE HEADER_FILES "*.h")


add_library(${This} STATIC ${SRC_FILES} ${HEADER_FILES})
