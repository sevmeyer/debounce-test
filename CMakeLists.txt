cmake_minimum_required(VERSION 3.20)

project(debounce)

# Language
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)

# Warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Executables
file(GLOB sources CONFIGURE_DEPENDS src/test*.c)
foreach(source ${sources})
    cmake_path(GET source STEM name)
    add_executable(${name} ${source})
endforeach()
