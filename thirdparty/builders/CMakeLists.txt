# imgui
file(GLOB IMGUI_SOURCE_FILES ./../imgui/*.cpp
                                     ./../imgui/*.c)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES})

target_link_libraries(imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../imgui)
target_include_directories(imgui PUBLIC ./../)


# rlImGui /raylib imgui/
file(GLOB_RECURSE RLIMGUI_SOURCE_FILES ./../rlImGui/*.cpp
                                       ./../rlImGui/*.c)

add_library(rlimgui STATIC ${RLIMGUI_SOURCE_FILES})

target_link_libraries(rlimgui PUBLIC raylib imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../rlImGui)
target_include_directories(rlimgui PUBLIC ./../)

# lua
file(GLOB LUA_SOURCE_FILES ./../lua/l*.cpp
                                   ./../lua/l*.c)

add_library(lua STATIC ${LUA_SOURCE_FILES})

target_link_libraries(lua)
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lua)
target_include_directories(lua PUBLIC ./../)
