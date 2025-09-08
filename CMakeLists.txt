cmake_minimum_required(VERSION 3.10)

project(sdltetris)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(main
	src/tetris.c
	src/collision.c
	src/initialize.c
	src/delrow.c
)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
target_link_libraries(main PRIVATE SDL3::SDL3)
