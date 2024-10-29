# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(MessageFragmentationLibrary)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(MessageFragmentationLibrary main.cpp MessageFragmenter.cpp)

# Include directories
target_include_directories(MessageFragmentationLibrary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Optionally, add any required libraries here
# target_link_libraries(MessageFragmenterApp some_library)
