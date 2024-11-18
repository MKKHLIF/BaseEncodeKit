set(This BaseCoderTests)

file(GLOB_RECURSE SRC_FILES "*.c")

find_package(GTest CONFIG REQUIRED)

add_executable(${This} ${SRC_FILES})

target_link_libraries(${This} PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(${This} PRIVATE BaseCoderLib)


include(GoogleTest)
gtest_discover_tests(${This})