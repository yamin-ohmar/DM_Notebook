
cmake_minimum_required(VERSION 3.20)

project(DM_Notebook LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find external dependencies
find_package(Crow CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# Support older CMake versions that use a different SQLite target name
if(NOT TARGET SQLite3::SQLite3)
    add_library(SQLite3::SQLite3 ALIAS SQLite::SQLite3)
endif()

# Define the executable and its C++ source files
add_executable(DM_Notebook
    backend/main.cpp
    backend/database/npc_repository.cpp
    backend/database/campaign_repository.cpp
    backend/routes/npc_routes.cpp
)

# Link external libraries
target_link_libraries(DM_Notebook PRIVATE
    Crow::Crow
    SQLite3::SQLite3
)


########################################

# Enable automated testing through CTest
enable_testing()

# Build the NPC repository test executable
add_executable(test_npc_repository
    tests/test_npc_repository.cpp
    backend/database/npc_repository.cpp
)

# Link SQLite (Crow is not needed for repository tests)
target_link_libraries(test_npc_repository PRIVATE
    SQLite3::SQLite3
)

# Register the executable with CTest
add_test(
    NAME NPCRepositoryTests
    COMMAND test_npc_repository
)

#########

# Build the CampaignRepository test executable
add_executable(test_campaign_repository
    tests/test_campaign_repository.cpp
    backend/database/campaign_repository.cpp
)

target_link_libraries(test_campaign_repository PRIVATE
    SQLite3::SQLite3
)

# Register campaign tests with CTest
add_test(
    NAME CampaignRepositoryTests
    COMMAND test_campaign_repository
)
