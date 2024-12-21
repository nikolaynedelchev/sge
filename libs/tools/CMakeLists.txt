file(GLOB_RECURSE TGT_SOURCE_FILES  ./*.cpp
                                    ./*.c)

add_library(tools STATIC ${TGT_SOURCE_FILES})

target_link_libraries(tools PUBLIC fmt)
target_include_directories(tools PUBLIC ./../)
