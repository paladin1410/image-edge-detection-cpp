cmake_minimum_required(VERSION 3.16)
project(EdgeDetection)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(third_party)

# Create executable from all source files
add_executable(edge_detector 
    src/main.cpp 
    src/Image.cpp
)
