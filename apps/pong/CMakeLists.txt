file(GLOB_RECURSE TGT_SOURCE_FILES  ./*.cpp
                                    ./*.c)

add_executable(pong ${TGT_SOURCE_FILES})

target_link_libraries(pong PRIVATE raylib tools fmt sge)
