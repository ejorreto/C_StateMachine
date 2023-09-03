cmake_minimum_required(VERSION 3.5)

# Project Name
project(C_StateMachine_test VERSION 0.1 LANGUAGES C)

# Configure output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compilation flags
set(CMAKE_C_FLAGS "-Wall")

# Create the static library
add_executable(c_statemachine_test main.c Motor.c CentrifugeTest.c)
target_link_libraries(c_statemachine_test c_statemachine_l)

