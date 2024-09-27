#include "task_manager.h"
#include <iostream>
#include <stdexcept>

TaskManager::TaskManager(const std::string& db_path) {
    std::cout << "Opening database at: " << db_path << std::endl;
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }
    initDatabase();
}

TaskManager::~TaskManager() {
    sqlite3_close(db);
}

void TaskManager::initDatabase() {
    const char* sql = "CREATE TABLE IF NOT EXISTS tasks ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "description TEXT NOT NULL,"
                      "deadline TEXT,"
                      "priority TEXT,"
                      "category TEXT"
                      ");";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "SQL error: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
    std::cout << "Database initialized successfully" << std::endl;
}

std::vector<Task> TaskManager::getTasks() {
    std::vector<Task> tasks;
    const char* sql = "SELECT id, description, deadline, priority, category FROM tasks;";
    sqlite3_stmt* stmt;

    std::cout << "Executing SQL: " << sql << std::endl;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        task.deadline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        task.priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        task.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        tasks.push_back(task);
        std::cout << "Retrieved task: " << task.id << " - " << task.description << std::endl;
    }

    sqlite3_finalize(stmt);
    std::cout << "Retrieved " << tasks.size() << " tasks" << std::endl;
    return tasks;
}

bool TaskManager::addTask(const Task& task) {
    const char* sql = "INSERT INTO tasks (description, deadline, priority, category) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, task.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, task.deadline.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, task.priority.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, task.category.c_str(), -1, SQLITE_STATIC);

    bool result = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    if (result) {
        std::cout << "Task added successfully" << std::endl;
    } else {
        std::cout << "Failed to add task" << std::endl;
    }
    
    return result;
}

bool TaskManager::updateTask(int id, const Task& task) {
    const char* sql = "UPDATE tasks SET description = ?, deadline = ?, priority = ?, category = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, task.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, task.deadline.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, task.priority.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, task.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);

    bool result = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return result;
}

bool TaskManager::deleteTask(int id) {
    const char* sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    bool result = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return result;
}
