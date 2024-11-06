#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

struct TodosListData {
    std::vector<std::string> todos;
    std::vector<int> todosIds;
};

TodosListData todos;
int selectedTodos = 0;
std::string newTodoName;
std::string newTodoDescription;

std::string dueYear;
std::string dueMonth;
std::string dueDay;
std::string dueHour;
std::string dueMinute;

/**
 * @brief Creates a limited input component.
 * 
 * This function creates a limited input component that restricts the length of the input content.
 * The input content is limited to the specified maximum length, and any excess characters are truncated.
 */
ftxui::Component limitedInput(std::string* content, const std::string& placeholder, int maxLength) {
    auto input = ftxui::Input(content, placeholder);

    return ftxui::Renderer(input, [content, input, maxLength]() {
        if (content->size() > static_cast<size_t>(maxLength)) {
            content->resize(maxLength);
        }
        return input->Render();
    });
}

/**
 * @brief Adds a new todo item to the list.
 * 
 * This function adds a new todo item to the list of todos.
 * The new todo item is created with the specified name, description, and due date.
 * The function returns the ID of the newly created todo item.
 */
int addTodo(FileData *data, std::string newTodoName, std::string newTodoDescription, Date dueDate) {
    TodoData newTodo;
    
    newTodo.id = data->todosData.size() + 1;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    Date createDate;
    createDate.day = tm.tm_mday;
    createDate.month = tm.tm_mon + 1;
    createDate.year = tm.tm_year + 1900;
    createDate.hour = tm.tm_hour;
    createDate.minute = tm.tm_min;
    createDate.second = tm.tm_sec;
    newTodo.createDate = createDate;

    newTodo.dueDate = dueDate;

    newTodo.todoName = newTodoName;
    newTodo.todoDescription = newTodoDescription;

    data->todosData.push_back(newTodo);
    return newTodo.id;
}

/**
 * @brief Removes a todo item from the list.
 * 
 * This function removes a todo item from the list of todos.
 * The todo item with the specified ID is removed from the list.
 */
void removeTodo(FileData *data, int id) {
    auto it = std::remove_if(data->todosData.begin(), data->todosData.end(), [id](const TodoData& todo) {
        return todo.id == id;
    });
    data->todosData.erase(it, data->todosData.end());
}

/**
 * @brief Marks a todo item as done.
 * 
 * This function marks a todo item as done or undone.
 * The todo item with the specified ID is marked as done if the done parameter is true,
 * and marked as undone if the done parameter is false.
 */
void markTodoDone(FileData *data, int id, bool done) {
    for (auto& todo : data->todosData) {
        if (todo.id == id) {
            if (done) {
                todo.todoName += " (done)";
            }
            else {
                todo.todoName.erase(todo.todoName.size() - 7);
            }
        }
    }
}

/**
 * @brief Creates the todos component.
 * 
 * This function creates the todos component, which displays the list of todos and allows the user to interact with them.
 * The component includes options to add new todos, remove existing todos, and mark todos as done.
 */
ftxui::Component todosComponent(FileData *data) {
    for (const auto& todo : data->todosData) {
        todos.todos.push_back(todo.todoName);
        todos.todosIds.push_back(todo.id);
    }

    auto todosList = ftxui::Menu(&todos.todos, &selectedTodos) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30);

    auto newTodoInput = ftxui::Input(&newTodoName, "New Todo");
    auto descriptionInput = ftxui::Input(&newTodoDescription, "Description");

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    dueYear = std::to_string(tm.tm_year + 1900);
    dueMonth = std::to_string(tm.tm_mon + 1);
    dueDay = std::to_string(tm.tm_mday);
    dueHour = std::to_string(tm.tm_hour);
    dueMinute = std::to_string(tm.tm_min);

    auto inputDueYear = limitedInput(&dueYear, "Year", 4) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5);
    auto inputDueMonth = limitedInput(&dueMonth, "Month", 2) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueDay = limitedInput(&dueDay, "Day", 2) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueHour = limitedInput(&dueHour, "Hour", 2) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueMinute = limitedInput(&dueMinute, "Minute", 2) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);

    auto dueDateInputs = ftxui::Container::Horizontal({
        inputDueYear,
        ftxui::Renderer([] { return ftxui::text("- "); }),
        inputDueMonth,
        ftxui::Renderer([] { return ftxui::text("- "); }),
        inputDueDay,
        ftxui::Renderer([] { return ftxui::text("/ "); }),
        inputDueHour,
        ftxui::Renderer([] { return ftxui::text(": "); }),
        inputDueMinute,
    });

    auto addButton = ftxui::Button("Add Todo", [data] {
        if (!newTodoName.empty()) {
            try {
                int newId = addTodo(data, newTodoName, newTodoDescription, {static_cast<short>(std::stoi(dueDay)), static_cast<short>(std::stoi(dueMonth)), static_cast<short>(std::stoi(dueYear)), static_cast<short>(std::stoi(dueHour)), static_cast<short>(std::stoi(dueMinute)), 0});
                todos.todos.push_back(newTodoName);
                todos.todosIds.push_back(newId);
                newTodoName.clear();
            } catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            }
        }
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20);

    auto removeButton = ftxui::Button("Remove Todo", [data] {
        if (!todos.todos.empty() && selectedTodos < static_cast<int>(todos.todos.size())) {
            int idToRemove = todos.todosIds[selectedTodos];
            todos.todos.erase(todos.todos.begin() + selectedTodos);
            todos.todosIds.erase(todos.todosIds.begin() + selectedTodos);
            selectedTodos = std::max(0, selectedTodos - 1);
            removeTodo(data, idToRemove);
        }
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20);

    auto markDoneButton = ftxui::Button("Mark Done", [data] {
        if (!todos.todos.empty() && selectedTodos < static_cast<int>(todos.todos.size())) {
            if (!todos.todos[selectedTodos].ends_with("(done)")) {
                todos.todos[selectedTodos] += " (done)"; 
                markTodoDone(data, todos.todosIds[selectedTodos], true);
            }
            else {
                todos.todos[selectedTodos].erase(todos.todos[selectedTodos].size() - 7);
                markTodoDone(data, todos.todosIds[selectedTodos], false);
            }
        }
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20);

    auto todosDisplay = ftxui::Renderer(todosList, [todosList] {
        if (todos.todos.empty()) {
            return ftxui::text("Todo list is empty");
        }
        return todosList->Render();
    });
    
    auto todosButtons = ftxui::Container::Horizontal({
        removeButton,
        markDoneButton,
    });

    auto selectedTodoDate = ftxui::Renderer([data] {
        if (todos.todos.empty() || selectedTodos >= static_cast<int>(todos.todos.size())) {
            return ftxui::text("Due Date: No Todo selected");
        }
        const auto& todo = data->todosData[selectedTodos];
        std::string dateText = "Due Date: " + std::to_string(todo.dueDate.year) + "-" +
                                std::to_string(todo.dueDate.month) + "-" +
                                std::to_string(todo.dueDate.day) + " " +
                                std::to_string(todo.dueDate.hour) + ":" +
                                std::to_string(todo.dueDate.minute);
        return ftxui::text(dateText);
    });

    auto selectedTodoDescription = ftxui::Renderer([data] {
        if (todos.todos.empty() || selectedTodos >= static_cast<int>(todos.todos.size())) {
            return ftxui::text("Description: No Todo selected");
        }
        const auto& todo = data->todosData[selectedTodos];
        return ftxui::text("Description: " + todo.todoDescription);
    });

    auto todosListLabel = Renderer([] {
        return text("Todos list:");
    });
    auto selectedTodoLabel = Renderer([] {
        return text("Selected Todo:");
    });
    auto addTodoLabel = Renderer([] {
        return text("Add Todo:");
    });

    return ftxui::Container::Horizontal({
        ftxui::Renderer([] { return filler(); }),
        ftxui::Container::Vertical({
            todosListLabel,
            todosDisplay,
        }),
        Renderer([]() -> Element {
            return hbox({
                filler() | size(WIDTH, EQUAL, 2),
                separator() | size(WIDTH, EQUAL, 1),
                filler() | size(WIDTH, EQUAL, 2),
            });
        }),
        ftxui::Container::Vertical({
            selectedTodoLabel,
            selectedTodoDescription,
            selectedTodoDate,
            ftxui::Renderer([] { return filler() | size(WIDTH, EQUAL, 1); }),
            todosButtons,
        }),
        Renderer([]() -> Element {
            return hbox({
                filler() | size(WIDTH, EQUAL, 2),
                separator() | size(WIDTH, EQUAL, 1),
                filler() | size(WIDTH, EQUAL, 2),
            });
        }),
        ftxui::Container::Vertical({
            addTodoLabel,
            ftxui::Renderer([] { return ftxui::text("Todo Title:"); }),
            newTodoInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30),
            ftxui::Renderer([] { return ftxui::text("Description:"); }),
            descriptionInput | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 45),
            ftxui::Renderer([] { return ftxui::text("Due Date:"); }),
            dueDateInputs,
            addButton,
        }),
        ftxui::Renderer([] { return filler(); }),
    });
}
