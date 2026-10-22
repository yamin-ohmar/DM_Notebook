
cmake_minimum_required(VERSION 3.20)

project(DM_Notebook LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find external dependencies
find_package(Crow CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# Define the executable and its C++ source files
add_executable(DM_Notebook
    backend/main.cpp
    backend/database/npc_repository.cpp
    backend/routes/npc_routes.cpp
)

# Link external libraries
target_link_libraries(DM_Notebook PRIVATE
    Crow::Crow
    SQLite3::SQLite3
)
