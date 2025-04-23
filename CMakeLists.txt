cmake_minimum_required(VERSION 3.10.0)
project(SDL_dungeon VERSION 0.1.0 LANGUAGES C CXX)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_executable(SDL_dungeon main.cpp)
target_include_directories(SDL_dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include/SDL3)
target_include_directories(SDL_dungeon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_link_libraries(SDL_dungeon PRIVATE SDL3::SDL3)
