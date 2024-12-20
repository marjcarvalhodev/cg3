cmake_minimum_required(VERSION 3.11)

project(memoria)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

enable_abcg(${PROJECT_NAME})