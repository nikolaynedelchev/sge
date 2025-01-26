cmake_minimum_required(VERSION 3.16)

# Име на проекта
project(SGE)

# Задаване на C++ стандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавяне на поддиректории
add_subdirectory(thirdparty/fmt)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/fmt)

add_subdirectory(thirdparty/glm)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/glm)

add_subdirectory(thirdparty/entt)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/entt/src)

add_subdirectory(thirdparty/raylib)
add_subdirectory(thirdparty/imguibuilder)

add_subdirectory(libs/tools)

add_subdirectory(libs/sge)

add_subdirectory(apps/dev_app)
add_subdirectory(apps/pong)
