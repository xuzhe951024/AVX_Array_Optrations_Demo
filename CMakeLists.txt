cmake_minimum_required(VERSION 3.21)
project(ECE697LS)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-mavx2)

add_executable(ECE697LS main.cpp sources/Metrix.cpp includes/Metrix.hpp)