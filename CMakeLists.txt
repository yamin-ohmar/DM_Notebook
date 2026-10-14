cmake_minimum_required(VERSION 3.20)

project(DM_Notebook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Crow CONFIG REQUIRED)

add_executable(DM_Notebook backend/main.cpp)

target_link_libraries(DM_Notebook PRIVATE Crow::Crow)
