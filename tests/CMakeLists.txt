set(This BaseCoderTests)

file(GLOB_RECURSE SRC_FILES "*.c")

add_executable(${This} ${SRC_FILES})

target_link_libraries(${This} PRIVATE unity)
target_link_libraries(${This} PRIVATE BaseCoderLib)
