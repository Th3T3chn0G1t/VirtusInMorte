cmake_minimum_required(VERSION 3.23)

cmake_policy(SET CMP0126 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0102 NEW)

project(Virtus)

set(EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# GLAD
    set(GLAD Source/Vendor/glad/src/gl.c)
    include_directories(Source/Vendor/glad/include)
#

# GLFW
    add_subdirectory(Source/Vendor/GLFW)
	link_libraries(glfw)
#

# STB
    include_directories(Source/Vendor/stb)
#

# Assimp
    set(ASSIMP_INSTALL OFF)
    add_subdirectory(Source/Vendor/assimp)
    link_libraries(assimp)
#

# FMT
    add_subdirectory(Source/Vendor/fmt)
    link_libraries(fmt)
#

# GLM
    add_subdirectory(Source/Vendor/glm)
    link_libraries(glm)
#

# Yaml-cpp
    add_subdirectory(Source/Vendor/yaml-cpp)
    link_libraries(yaml-cpp)
#

# Nuklear
    include_directories(Source/Vendor/Nuklear)
    include_directories(Source/Vendor/Nuklear/demo/glfw_opengl3)
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

# ASIO
    include_directories(Source/Vendor/asio/asio/include)
#

# Common
    file(GLOB Common Source/Common/*.cpp Source/Include/*.hpp)
    include_directories(Source/Include)
#

# Virtus
    file(GLOB Virtus Source/Virtus/*.cpp Source/Include/*.hpp Source/Virtus/SingleHeaderImplementation.c)
    include_directories(Source/Include)

    add_executable(Virtus ${Virtus} ${Common} ${GLAD})

    target_precompile_headers(Virtus PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:<Virtus.hpp$<ANGLE-R>>")
    set_property(SOURCE Source/Virtus/SingleHeaderImplementation.c PROPERTY SKIP_PRECOMPILE_HEADERS)
#
