#include "crow.h"
#include "task_manager.h"

int main()
{
    crow::SimpleApp app;

    // Route to fetch all tasks
    CROW_ROUTE(app, "/tasks")
    ([]()
     {
         auto tasks = TaskManager::getTasks();
         crow::json::wvalue json_result;
         crow::json::wvalue::list task_list; // Create a temporary list

         for (const auto &task : tasks)
         {
             task_list.push_back(crow::json::wvalue(task)); // Add each task as a wvalue
         }

         json_result["tasks"] = std::move(task_list); // Move the list into the JSON result
         return json_result;                          // Return as JSON
     });

    // Route to add a task
    CROW_ROUTE(app, "/add_task").methods("POST"_method)([](const crow::request &req)
    {
        auto task = crow::json::load(req.body);
        if (!task) {
            CROW_LOG_INFO << "Invalid task data";  // Corrected typo in the log message
            return crow::response(400);
        }

        std::string description = task["description"].s();
        CROW_LOG_INFO << "Received task description: " << description;

        TaskManager::addTask(description);  // Use the extracted 'description'
        CROW_LOG_INFO << "Task added successfully";
        return crow::response(200);
    });

    // Route to delete a task
    CROW_ROUTE(app, "/delete_task").methods("POST"_method)([](const crow::request &req)
    {
        auto task = crow::json::load(req.body);
        if (!task) {
            return crow::response(400);
        }
        int taskId = task["id"].i();
        TaskManager::deleteTask(taskId);  // Implement deleteTask in task_manager.cpp
        CROW_LOG_INFO << "Task deleted successfully with ID: " << taskId;
        return crow::response(200);
    });

    // Start the server
    app.port(18080).multithreaded().run();
}

