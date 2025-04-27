cmake_minimum_required(VERSION 3.10.0)
project(SDL_dungeon VERSION 0.1.0 LANGUAGES C CXX)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(utils/SDL EXCLUDE_FROM_ALL)


aux_source_directory(src SOURCE)
add_executable(SDL_dungeon ${SOURCE}) # 稍后删除main.cpp

target_include_directories(SDL_dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils/SDL/include/SDL3)
target_link_libraries(SDL_dungeon PRIVATE SDL3::SDL3)
target_include_directories(SDL_dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils/json/single_include/nlohmann)
target_include_directories(SDL_dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils/stb)

target_include_directories(SDL_dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
