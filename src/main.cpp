#include "crow.h"
#include "task_manager.h"
#include <filesystem>
#include <iostream>

int main() {
    crow::SimpleApp app;
    
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path public_dir = cwd.parent_path() / "public";
    std::filesystem::path db_path = cwd.parent_path() / "tasks.db";

    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Public directory: " << public_dir << std::endl;
    std::cout << "Database path: " << db_path << std::endl;

    TaskManager taskManager(db_path.string());

    CROW_ROUTE(app, "/")(
        [&public_dir]() {
            crow::response res;
            res.set_static_file_info(public_dir / "index.html");
            return res;
        }
    );

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)(
        [&taskManager]() {
            std::cout << "Received request for /tasks" << std::endl;
            auto tasks = taskManager.getTasks();
            crow::json::wvalue json_tasks;
            for (size_t i = 0; i < tasks.size(); ++i) {
                const auto& task = tasks[i];
                crow::json::wvalue json_task;
                json_task["id"] = task.id;
                json_task["description"] = task.description;
                json_task["deadline"] = task.deadline;
                json_task["priority"] = task.priority;
                json_task["category"] = task.category;
                json_tasks[i] = std::move(json_task);
            }
            std::cout << "Returning " << tasks.size() << " tasks" << std::endl;
            return crow::response(json_tasks);
        }
    );

    CROW_ROUTE(app, "/add_task").methods(crow::HTTPMethod::POST)(
        [&taskManager](const crow::request& req) {
            std::cout << "Received request to add task" << std::endl;
            auto json = crow::json::load(req.body);
            if (!json) {
                std::cout << "Invalid JSON received" << std::endl;
                return crow::response(400, "Invalid JSON");
            }

            Task task;
            task.description = json["description"].s();
            task.deadline = json["deadline"].s();
            task.priority = json["priority"].s();
            task.category = json["category"].s();

            if (taskManager.addTask(task)) {
                std::cout << "Task added successfully" << std::endl;
                return crow::response(200);
            } else {
                std::cout << "Failed to add task" << std::endl;
                return crow::response(500, "Failed to add task");
            }
        }
    );

    CROW_ROUTE(app, "/update_task").methods(crow::HTTPMethod::POST)(
        [&taskManager](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }

            int id = json["id"].i();
            Task task;
            task.description = json["description"].s();
            task.deadline = json["deadline"].s();
            task.priority = json["priority"].s();
            task.category = json["category"].s();

            if (taskManager.updateTask(id, task)) {
                return crow::response(200);
            } else {
                return crow::response(500, "Failed to update task");
            }
        }
    );

    CROW_ROUTE(app, "/delete_task").methods(crow::HTTPMethod::POST)(
        [&taskManager](const crow::request& req) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }

            int id = json["id"].i();

            if (taskManager.deleteTask(id)) {
                return crow::response(200);
            } else {
                return crow::response(500, "Failed to delete task");
            }
        }
    );

    // This should be the last route
    CROW_ROUTE(app, "/<path>")
    ([&public_dir](const std::string& path) {
        crow::response res;
        res.set_static_file_info(public_dir / path);
        return res;
    });

    app.loglevel(crow::LogLevel::Debug);
    app.port(18080).multithreaded().run();
    return 0;
}
