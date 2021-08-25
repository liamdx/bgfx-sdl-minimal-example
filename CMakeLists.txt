cmake_minimum_required(VERSION 3.8)
project(bgfx-sdl-minimal-example)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCE_FILES "${SOURCE_DIR}/main.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})