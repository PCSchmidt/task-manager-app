cmake_minimum_required(VERSION 3.10)

# Project name
project(TaskManagerApp)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add Crow include directories (point to the local 'crow' folder)
include_directories(${PROJECT_SOURCE_DIR}/crow/include)

# Add the executable
add_executable(TaskManagerApp src/main.cpp src/task_manager.cpp)

# Link with SQLite3 and Crow (no need for boost_system if Crow is local)
target_link_libraries(TaskManagerApp sqlite3)
