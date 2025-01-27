file(GLOB_RECURSE PONG_SOURCE_FILES  ./*.cpp
                                     ./*.c)

add_executable(pong ${PONG_SOURCE_FILES})

target_link_libraries(pong PUBLIC raylib tools fmt sge rlimgui lua)
