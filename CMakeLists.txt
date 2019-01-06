cmake_minimum_required(VERSION 3.12)

project(binarystream)

set(CMAKE_CXX_STANDARD 14)

add_library(binarystream include/BinaryStream.h src/BinaryStream.cpp)
target_include_directories(binarystream PUBLIC include/)