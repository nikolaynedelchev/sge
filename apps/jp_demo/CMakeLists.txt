file(GLOB_RECURSE JP_DEMO_SOURCE_FILES  ./*.cpp
                                        ./*.c)

add_executable(jp_demo ${JP_DEMO_SOURCE_FILES})

target_link_libraries(jp_demo PRIVATE tools fmt libuv::libuv uvw)
