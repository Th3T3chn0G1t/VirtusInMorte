cmake_minimum_required(VERSION 3.23)

project(Virtus)

set(EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# GLAD
    set(GLAD Virtus/Vendor/glad/src/gl.c)
    include_directories(Virtus/Vendor/glad/include)
	link_libraries(GL)
#

# GLFW
	link_libraries(glfw)
#

# FMT
    link_libraries(fmt)
#

file(GLOB Virtus Virtus/*.cpp Virtus/Include/*.hpp)
include_directories(Virtus/Include)
add_executable(Virtus ${Virtus} ${GLAD})
target_precompile_headers(Virtus PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:<Virtus.hpp$<ANGLE-R>>")
