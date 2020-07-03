cmake_minimum_required(VERSION 3.10)

project(debounce LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verbose compiler warnings
if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Executable
add_executable(debounce "src/main.cpp")
