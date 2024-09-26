# Task Manager App

## Overview
This Task Manager App is a simple web-based application built using C++ and Crow, a lightweight web framework. It uses SQLite for task storage and allows users to create, retrieve, and manage tasks through API routes.

## Features
- **Create Task**: Add new tasks by sending a POST request.
- **Retrieve Tasks**: View all tasks in the database through a GET request.
- **Delete Task**: (Upcoming) Delete a task from the list by providing its ID.
- **Update Task**: (Upcoming) Edit a task’s description by providing its ID and new description.
- **Mark Task as Complete**: (Upcoming) Mark tasks as completed.

## Technologies Used
- **C++**
- **Crow Web Framework**
- **SQLite** for database storage
- **WSL (Ubuntu)** for local development

## Setup Instructions

### Prerequisites
- **C++ Compiler** (g++ or clang++)
- **CMake** for build configuration
- **SQLite3** for database
- **Crow** for web server

### Installation

1. Clone the repository:
   ```bash
    git clone https://github.com/PCSchmidt/task-manager-app.git
    cd task-manager-app


2. Build the project using CMake
    mkdir build
    cd build
    cmake ..
    make

3. Run the app
    ./TaskManagerApp

4. Access the application: Open your browser and navigate to http://localhost:18080/tasks

### API End Points
- GET /tasks: Retrieve all tasks
curl http://localhost:18080/tasks

- POST /add_task: Add a new task. You need to pass a JSON object with the task description.
curl -X POST http://localhost:18080/add_task -d '{"description":"New Task"}' -H "Content-Type: application/json"

### Future Enhancements
- Task deletion
- Task update
- Mark tasks as complete
- Frontend interface for task management

### License
- This project is licensed under the MIT License.

