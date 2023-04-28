cmake_minimum_required(VERSION 3.5)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
project(EAFramework VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(EAFramework 
                ${CMAKE_SOURCE_DIR}/src/main.cpp
                ${CMAKE_SOURCE_DIR}/src/population/population.hpp
                ${CMAKE_SOURCE_DIR}/src/population/population_so.hpp
                ${CMAKE_SOURCE_DIR}/src/population/population_mo.hpp
            )                      

target_compile_features(EAFramework PUBLIC cxx_std_20)
target_include_directories(EAFramework PRIVATE include)