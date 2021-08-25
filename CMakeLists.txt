cmake_minimum_required(VERSION 3.14)

project(bgfx-sdl-minimal-example)
# We set the C standard to 17.
set (CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/SDL2)
set(BGFX_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/bgfx.cmake)

set(SOURCE_FILES "${SOURCE_DIR}/main.cpp")

add_subdirectory(${SDL_DIR})
add_subdirectory(${BGFX_CMAKE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} bgfx)
target_include_directories(${PROJECT_NAME} PUBLIC "${SDL_DIR}/include" "${BGFX_CMAKE_DIR}/bgfx/include" "${BGFX_CMAKE_DIR}/bx/include" "${BGFX_CMAKE_DIR}/bimg/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
