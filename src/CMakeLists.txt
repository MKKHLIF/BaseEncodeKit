set(This BaseCoderLib)

file(GLOB_RECURSE SRC_FILES "*.c")
file(GLOB_RECURSE HEADER_FILES "*.h")


add_library(${This} STATIC ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${This} PUBLIC ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(${This} PRIVATE ${CMAKE_SOURCE_DIR}/src/internal)