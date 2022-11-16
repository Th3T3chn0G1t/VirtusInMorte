cmake_minimum_required(VERSION 3.23)

project(Virtus)

set(EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB Virtus Virtus/*.cpp Virtus/Include/*.hpp)
add_executable(Virtus ${Virtus})
