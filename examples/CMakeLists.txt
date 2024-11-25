add_executable(base64 base64.c)
add_executable(base32 base32.c)

target_link_libraries(base64 PRIVATE BaseCoderLib)
target_link_libraries(base32 PRIVATE BaseCoderLib)
