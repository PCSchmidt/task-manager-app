#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include <sqlite3.h>

class TaskManager {
public:
    static std::vector<std::string> getTasks();
    static void addTask(const std::string& description);
    static void deleteTask(int id);  // Add this line to declare the deleteTask function
};

#endif  // TASK_MANAGER_H
