set(This Base64Encoder)

file(GLOB_RECURSE SRC_FILES "*.c")

add_executable(${This} ${SRC_FILES})

target_link_libraries(${This} PRIVATE BaseEncoderLib)