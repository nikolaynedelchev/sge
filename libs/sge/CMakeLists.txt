file(GLOB_RECURSE TGT_SOURCE_FILES  ./*.cpp
                                    ./*.c)

add_library(sge STATIC ${TGT_SOURCE_FILES})

target_link_libraries(sge PUBLIC raylib fmt)
target_include_directories(sge PUBLIC ./../)
