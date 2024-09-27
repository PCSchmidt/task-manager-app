document.addEventListener('DOMContentLoaded', () => {
    const addTaskForm = document.getElementById('add-task-form');
    const taskList = document.getElementById('task-list');

    loadTasks();

    addTaskForm.addEventListener('submit', (e) => {
        e.preventDefault();
        const description = document.getElementById('task-description').value;
        const deadline = document.getElementById('task-deadline').value;
        const priority = document.getElementById('task-priority').value;
        const category = document.getElementById('task-category').value;

        addTask({ description, deadline, priority, category });
    });

    async function loadTasks() {
        const response = await fetch('/tasks');
        const tasks = await response.json();
        renderTasks(tasks);
    }

    async function addTask(task) {
        const response = await fetch('/add_task', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(task),
        });

        if (response.ok) {
            addTaskForm.reset();
            loadTasks();
        }
    }

    async function deleteTask(id) {
        const response = await fetch('/delete_task', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ id }),
        });

        if (response.ok) {
            loadTasks();
        }
    }

    async function updateTask(id, task) {
        const response = await fetch('/update_task', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ id, ...task }),
        });

        if (response.ok) {
            loadTasks();
        }
    }

    function renderTasks(tasks) {
        taskList.innerHTML = '';
        tasks.forEach((task) => {
            const taskElement = document.createElement('div');
            taskElement.className = `task priority-${task.priority}`;
            taskElement.innerHTML = `
                <div class="task-info">
                    <strong>${task.description}</strong>
                    <p>Deadline: ${task.deadline || 'Not set'}</p>
                    <p>Category: ${task.category || 'Uncategorized'}</p>
                </div>
                <div class="task-actions">
                    <button onclick="deleteTask(${task.id})">Delete</button>
                    <button onclick="editTask(${task.id})">Edit</button>
                </div>
            `;
            taskList.appendChild(taskElement);
        });
    }

    window.deleteTask = deleteTask;

    window.editTask = (id) => {
        const taskElement = document.querySelector(`.task[data-id="${id}"]`);
        const description = taskElement.querySelector('strong').textContent;
        const deadline = taskElement.querySelector('p:nth-child(2)').textContent.split(': ')[1];
        const category = taskElement.querySelector('p:nth-child(3)').textContent.split(': ')[1];

        const newDescription = prompt('Edit task description:', description);
        const newDeadline = prompt('Edit deadline:', deadline);
        const newCategory = prompt('Edit category:', category);

        if (newDescription !== null) {
            updateTask(id, {
                description: newDescription,
                deadline: newDeadline,
                category: newCategory,
            });
        }
    };
});
