cmake_minimum_required(VERSION 3.23)

project(Virtus)

set(EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# GLAD
    set(GLAD Virtus/Vendor/glad/src/gl.c)
    include_directories(Virtus/Vendor/glad/include)
#

# GLFW
	link_libraries(glfw)
#

# FMT
    link_libraries(fmt)
#

# GLM
#

# STB
#

# Assimp
    link_libraries(assimp)
#

# Yaml-cpp
    link_libraries(yaml-cpp)
#

# Nuklear
    include_directories(Virtus/Vendor/nuklear)
    add_compile_definitions(
            NK_INCLUDE_FIXED_TYPES
            NK_INCLUDE_DEFAULT_ALLOCATOR
            NK_INCLUDE_STANDARD_IO
            NK_INCLUDE_STANDARD_VARARGS
            NK_INCLUDE_STANDARD_BOOL
            NK_INCLUDE_VERTEX_BUFFER_OUTPUT
            NK_INCLUDE_FONT_BAKING
            NK_INCLUDE_DEFAULT_FONT
            NK_INCLUDE_COMMAND_USERDATA
    )
#

file(GLOB Virtus Virtus/*.cpp Virtus/*.c Virtus/Include/*.hpp)
set_property(SOURCE Virtus/SingleHeaderImplementation.c PROPERTY SKIP_PRECOMPILE_HEADERS)
include_directories(Virtus/Include)
add_executable(Virtus ${Virtus} ${GLAD})
target_precompile_headers(Virtus PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:<Virtus.hpp$<ANGLE-R>>")
