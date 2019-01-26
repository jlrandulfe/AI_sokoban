# CMake version.
cmake_minimum_required(VERSION 2.8.11)

# The name of the project.
project(sokoban-solver)

# Used to resolve absolute path names
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Use the c++11 standard
set (CMAKE_CXX_STANDARD 11)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}/src")

# Use c++11 and check OpenCV 3.
SET(CMAKE_CXX_FLAGS "-fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}
							   -fno-omit-frame-pointer -fsanitize=address")

SET(SOURCEFILES src/main.cpp src/sokoban.cpp src/sokoban.hpp)

add_executable(sokoban-solver ${SOURCEFILES})

MESSAGE(${PROJECT_NAME} " done!")
