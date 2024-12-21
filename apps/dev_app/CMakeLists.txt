file(GLOB_RECURSE TGT_SOURCE_FILES  ./*.cpp
                                    ./*.c)

add_executable(dev_app ${TGT_SOURCE_FILES})

target_link_libraries(dev_app PRIVATE raylib tools fmt sge)
