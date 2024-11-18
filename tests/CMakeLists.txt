set(This BaseCoderTests)

file(GLOB_RECURSE SRC_FILES "*.c")

add_executable(${This} ${SRC_FILES})

target_include_directories(${This} PUBLIC external/unity/src)
#target_link_directories(${This} PUBLIC external/unity/src)

target_link_libraries(${This} PRIVATE unity)
