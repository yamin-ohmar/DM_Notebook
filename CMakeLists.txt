cmake_minimum_required(VERSION 3.20)

project(DM_Grimoire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DM_Grimoire backend/main.cpp)
