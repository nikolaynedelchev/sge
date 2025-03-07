file(GLOB_RECURSE TESTS_APP_SOURCE_FILES  ./*.cpp
                                          ./*.c)

add_executable(tests_app ${TESTS_APP_SOURCE_FILES})

target_link_libraries(tests_app PRIVATE tools fmt)
