cmake_minimum_required(VERSION 3.10)
project(TaskManagerApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite package
find_package(SQLite3 REQUIRED)

# Add Crow
add_subdirectory(external/crow)

# Add source files
set(SOURCES
    src/main.cpp
    src/task_manager.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3 Crow::Crow)

# Add compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Copy public directory to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/public $<TARGET_FILE_DIR:${PROJECT_NAME}>/public
)
