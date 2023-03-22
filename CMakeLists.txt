cmake_minimum_required(VERSION 3.5)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
project(EAFramework VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_executable(EAFramework 
                ${CMAKE_SOURCE_DIR}/src/main.cpp
                ${CMAKE_SOURCE_DIR}/src/population.cpp
                ${CMAKE_SOURCE_DIR}/src/population.hpp
                ${CMAKE_SOURCE_DIR}/src/operators_so.hpp
                ${CMAKE_SOURCE_DIR}/src/population_so.cpp
                ${CMAKE_SOURCE_DIR}/src/population_so.hpp
                ${CMAKE_SOURCE_DIR}/src/population_mo.cpp
                ${CMAKE_SOURCE_DIR}/src/population_mo.hpp
            )                      

target_include_directories(EAFramework PRIVATE include)