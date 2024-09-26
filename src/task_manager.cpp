#include "task_manager.h"
#include <iostream>

// Function to retrieve tasks from the SQLite database
std::vector<std::string> TaskManager::getTasks() {
    sqlite3* db;
    int rc = sqlite3_open("../db/tasks.db", &db);  // Open the database
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return {};  // Return an empty list in case of failure
    } else {
        std::cout << "Database opened successfully (for getting tasks)" << std::endl;
    }

    std::vector<std::string> tasks;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT description FROM tasks";  // SQL query to fetch task descriptions
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string task = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        tasks.push_back(task);
    }

    sqlite3_finalize(stmt);  // Clean up the statement
    sqlite3_close(db);  // Close the database

    return tasks;
}

// Function to add a task to the SQLite database
void TaskManager::addTask(const std::string& description) {
    sqlite3* db;
    int rc = sqlite3_open("../db/tasks.db", &db);  // Open the database
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    } else {
        std::cout << "Database opened successfully (for adding task)" << std::endl;
    }

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO tasks (description) VALUES (?)";  // SQL query to insert a new task

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);  // Prepare the SQL statement
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    } else {
        std::cout << "SQL statement prepared successfully" << std::endl;
    }

    rc = sqlite3_bind_text(stmt, 1, description.c_str(), -1, SQLITE_STATIC);  // Bind the task description
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to bind text: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Description bound successfully" << std::endl;
    }

    rc = sqlite3_step(stmt);  // Execute the statement
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Task added successfully!" << std::endl;
    }

    sqlite3_finalize(stmt);  // Clean up
    sqlite3_close(db);  // Close the database
}

// Function to delete a task by its ID
void TaskManager::deleteTask(int id) {
    sqlite3* db;
    int rc = sqlite3_open("../db/tasks.db", &db);  // Open the database
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    } else {
        std::cout << "Database opened successfully (for deleting task)" << std::endl;
    }

    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM tasks WHERE id = ?";  // SQL query to delete a task by its ID

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);  // Prepare the SQL statement
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    } else {
        std::cout << "SQL statement prepared successfully" << std::endl;
    }

    rc = sqlite3_bind_int(stmt, 1, id);  // Bind the task ID to the query
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to bind ID: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Task ID bound successfully" << std::endl;
    }

    rc = sqlite3_step(stmt);  // Execute the statement
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Task deleted successfully!" << std::endl;
    }

    sqlite3_finalize(stmt);  // Clean up
    sqlite3_close(db);  // Close the database
}
