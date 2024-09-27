#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>
#include <sqlite3.h>

struct Task {
    int id;
    std::string description;
    std::string deadline;
    std::string priority;
    std::string category;
};

class TaskManager {
public:
    TaskManager(const std::string& db_path);
    ~TaskManager();

    std::vector<Task> getTasks();
    bool addTask(const Task& task);
    bool updateTask(int id, const Task& task);
    bool deleteTask(int id);

private:
    sqlite3* db;
    void initDatabase();
};

#endif // TASK_MANAGER_H
