file(GLOB_RECURSE RSSEDIT_SOURCE_FILES  ./*.cpp
                                        ./*.c)

add_executable(rssedit ${RSSEDIT_SOURCE_FILES})

target_link_libraries(rssedit PUBLIC raylib tools fmt sge rlimgui)
