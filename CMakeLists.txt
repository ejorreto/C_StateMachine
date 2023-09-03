cmake_minimum_required(VERSION 3.5)

# Project Name
project(C_StateMachine_lib VERSION 0.1 LANGUAGES C)

# Configure output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compilation flags
set(CMAKE_C_FLAGS "-Wall")

# Create the static library
add_library(c_statemachine_l STATIC Fault.c fb_allocator.c LockGuard.c sm_allocator.c StateMachine.c x_allocator.c)
target_include_directories(c_statemachine_l PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(test)

############# Custom targets #############

# Remove all files in cmake binary dir for a 
add_custom_target(mrproper
    COMMENT "Removing all fines in ${CMAKE_BINARY_DIR}"
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
    )