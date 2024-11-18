set(This BaseCoderTests)

file(GLOB_RECURSE SRC_FILES "*.c")

add_executable(${This} ${SRC_FILES})

find_package(cmocka CONFIG REQUIRED)

target_link_libraries(${This} PRIVATE cmocka::cmocka)
target_link_libraries(${This} PRIVATE BaseCoderLib)
