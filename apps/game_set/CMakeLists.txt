file(GLOB_RECURSE GAME_SET_SOURCE_FILES  ./*.cpp
                                         ./*.c)

add_executable(game_set ${GAME_SET_SOURCE_FILES})

target_link_libraries(game_set PRIVATE tools fmt)
