#include "todosComponent.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../Storage/localStorage.h"
#include "../mainThread.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

TodosComponent::TodosComponent() {
    selectedTodos = 0;
}

/**
 * @brief Creates a limited range input component.
 * 
 * This function creates a limited range input component for an integer value.
 * The input component is limited to the specified range of values.
 */
ftxui::Component TodosComponent::limitedRangeInput(std::string* content, const std::string& placeholder, int minValue, int maxValue) {
    auto input = ftxui::Input(content, placeholder);
    
    return ftxui::Renderer(input, [content, input, minValue, maxValue]() {
        if (!content->empty()) {
            try {
                int value = std::stoi(*content);
                if (value < minValue) {
                    *content = std::to_string(minValue);
                }
                else if (value > maxValue) {
                    *content = std::to_string(maxValue);
                } 
            } catch (const std::invalid_argument&) {
                *content = std::to_string(minValue);
            }
        }
        return input->Render();
    });
}

/**
 * @brief Creates a limited range input component for the day of the month.
 * 
 * This function creates a limited range input component for the day of the month.
 * The input component is limited to the range of days in the specified month.
 */
ftxui::Component TodosComponent::limitedRangeInputDay(std::string* content, const std::string& placeholder) {
    auto input = ftxui::Input(content, placeholder);

    return ftxui::Renderer(input, [content, input, this]() {
        if (!content->empty()) {
            int minValue = 1;
            int maxValue = 31;

            try {
                int month = std::stoi(this->dueMonth);
                if (month == 2) {
                    int year = std::stoi(this->dueYear);
                    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                        maxValue = 29;
                    } else {
                        maxValue = 28;
                    }
                } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                    maxValue = 30;
                }
                int value = std::stoi(*content);
                if (value < minValue) {
                    *content = std::to_string(minValue);
                }
                else if (value > maxValue) {
                    *content = std::to_string(maxValue);
                } 
            } catch (const std::invalid_argument&) {
                *content = std::to_string(minValue);
            }
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
int TodosComponent::addTodo(FileData *data, Date dueDate) {
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

    newTodo.todoName = this->newTodoName;
    newTodo.todoDescription = this->newTodoDescription;

    data->todosData.push_back(newTodo);
    return newTodo.id;
}

/**
 * @brief Removes a todo item from the list.
 * 
 * This function removes a todo item from the list of todos.
 * The todo item with the specified ID is removed from the list.
 */
void TodosComponent::removeTodo(FileData *data, int id) {
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
void TodosComponent::markTodoDone(FileData *data, int id, bool done) {
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
ftxui::Component TodosComponent::renderTodosComponent(FileData *data) {
    if (!data) return ftxui::Renderer([] { return ftxui::text("Error: Data is null"); });

    todos.todos.clear();
    todos.todosIds.clear();

    for (const auto& todo : data->todosData) {
        todos.todos.push_back(todo.todoName);
        todos.todosIds.push_back(todo.id);
    }

    auto todosList = ftxui::Menu(&todos.todos, &selectedTodos) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30);

    auto newTodoInput = ftxui::Input(&newTodoName, "New Todo");
    auto descriptionInput = ftxui::Input(&newTodoDescription, "Description");

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    this->dueYear = std::to_string(tm.tm_year + 1900);
    this->dueMonth = std::to_string(tm.tm_mon + 1);
    this->dueDay = std::to_string(tm.tm_mday + 1);
    this->dueHour = std::to_string(tm.tm_hour);
    this->dueMinute = std::to_string(tm.tm_min);

    auto inputDueYear = limitedRangeInput(&dueYear, "Year", 1900, 2100) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5);
    auto inputDueMonth = limitedRangeInput(&dueMonth, "Month", 1, 12) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueDay = limitedRangeInputDay(&dueDay, "Day") | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueHour = limitedRangeInput(&dueHour, "Hour", 0, 23) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);
    auto inputDueMinute = limitedRangeInput(&dueMinute, "Minute", 1, 60) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3);

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

    auto addButton = ftxui::Button("Add Todo", [data, this] {
        if (!newTodoName.empty()) {
            try {
                int newId = addTodo(data, {static_cast<short>(std::stoi(dueDay)), static_cast<short>(std::stoi(dueMonth)), static_cast<short>(std::stoi(dueYear)), static_cast<short>(std::stoi(dueHour)), static_cast<short>(std::stoi(dueMinute)), 0});
                todos.todos.push_back(newTodoName);
                todos.todosIds.push_back(newId);
                newTodoName.clear();
                newTodoDescription.clear();
            } catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            }
        }
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20);

    auto removeButton = ftxui::Button("Remove Todo", [data, this] {
        if (!todos.todos.empty() && selectedTodos < static_cast<int>(todos.todos.size())) {
            int idToRemove = todos.todosIds[selectedTodos];
            todos.todos.erase(todos.todos.begin() + selectedTodos);
            todos.todosIds.erase(todos.todosIds.begin() + selectedTodos);
            this->selectedTodos = std::max(0, this->selectedTodos - 1);
            removeTodo(data, idToRemove);
        }
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20);

    auto markDoneButton = ftxui::Button("Mark Done", [data, this] {
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

    auto todosDisplay = ftxui::Renderer(todosList, [todosList, this] {
        if (todos.todos.empty()) {
            return ftxui::text("Todo list is empty");
        }
        return ftxui::vbox({
            todosList->Render() | ftxui::vscroll_indicator | ftxui::frame,
        });
    });
    
    auto todosButtons = ftxui::Container::Horizontal({
        removeButton,
        markDoneButton,
    });

    auto selectedTodoDate = ftxui::Renderer([data, this] {
        if (todos.todos.empty() || selectedTodos >= static_cast<int>(todos.todos.size())) {
            return ftxui::text("Due Date: No Todo selected");
        }
        const auto& todo = data->todosData[selectedTodos];

        int month = todo.dueDate.month;
        std::string monthText = std::to_string(month);
        int day = todo.dueDate.day;
        std::string dayText = std::to_string(day);
        int hour = todo.dueDate.hour;
        std::string hourText = std::to_string(hour);
        int minute = todo.dueDate.minute;
        std::string minuteText = std::to_string(minute);

        if (month <= 9) {
            monthText = "0" + monthText;
        }
        if (day <= 9) {
            dayText = "0" + dayText;
        }
        if (hour <= 9) {
            hourText = "0" + hourText;
        }
        if (minute <= 9) {
            minuteText = "0" + minuteText;
        }

        std::string dateText = "Due Date: " + std::to_string(todo.dueDate.year) + "-" +
                                monthText + "-" + dayText + " " +
                                hourText + ":" + minuteText;
        return ftxui::text(dateText);
    });

    auto selectedTodoDescription = ftxui::Renderer([data, this] {
        if (todos.todos.empty() || selectedTodos >= static_cast<int>(todos.todos.size())) {
            return ftxui::text("Description: No Todo selected");
        }
        const auto& todo = data->todosData[selectedTodos];
        std::string description = todo.todoDescription;
        if (description.empty()) {
            description = "No description";
        }
        return ftxui::text("Description: " + description);
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
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 50),
        Renderer([]() -> Element {
            return hbox({
                filler() | size(WIDTH, EQUAL, 2),
                separator() | size(WIDTH, EQUAL, 1),
                filler() | size(WIDTH, EQUAL, 2),
            });
        }),
        ftxui::Container::Vertical({
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
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Renderer([] { return ftxui::separatorEmpty(); }),
            ftxui::Container::Vertical({
                selectedTodoLabel,
                selectedTodoDescription,
                selectedTodoDate,
                ftxui::Renderer([] { return filler() | size(WIDTH, EQUAL, 1); }),
                todosButtons,
            }),
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 50),
        ftxui::Renderer([] { return filler(); }),
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 180) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 40);
}
