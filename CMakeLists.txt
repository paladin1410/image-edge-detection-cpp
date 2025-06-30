cmake_minimum_required(VERSION 3.16)
project(EdgeDetection)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable from main.cpp
add_executable(edge_detector src/main.cpp)
