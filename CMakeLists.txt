cmake_minimum_required(VERSION 3.29)

project(BaseEncodeKit C)

set(CMAKE_C_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_package(Git QUIET)

if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Option to enable/disable submodule updates
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if (GIT_SUBMODULE)
        # Update submodules recursively
        execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT
        )

        if (NOT GIT_SUBMODULE_RESULT EQUAL 0)
            message(FATAL_ERROR "Error updating submodules: ${GIT_SUBMODULE_RESULT}")
        endif ()
    endif ()
endif ()

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/unity/CMakeLists.txt")
    message(FATAL_ERROR "unity submodule is missing. Please initialize and update submodules.")
endif ()


add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(external/unity)
include(CTest)
