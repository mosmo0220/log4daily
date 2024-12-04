#ifndef TODOS_COMPONENT_H
#define TODOS_COMPONENT_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../Storage/localStorage.h"
#include "../applicationManager.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

struct TodosListData {
    std::vector<std::string> todos;
    std::vector<int> todosIds;
};

class TodosComponent {
public:
    TodosComponent();
    
    /**
     * @brief Creates a limited range input component.
     * 
     * This function creates a limited range input component for an integer value.
     */
    ftxui::Component limitedRangeInput(std::string* content, const std::string& placeholder, int minValue, int maxValue);
    /**
     * @brief Creates a limited range input component for the day of the month.
     * 
     * This function creates a limited range input component for the day of the month.
     */
    ftxui::Component limitedRangeInputDay(std::string* content, const std::string& placeholder);
    /**
     * @brief Adds a new todo item to the list.
     * 
     * This function adds a new todo item to the list of todos.
     */
    int addTodo(FileData *data, Date dueDate);
    /**
     * @brief Removes a todo item from the list.
     * 
     * This function removes a todo item from the list of todos.
     */
    void removeTodo(FileData *data, int id);
    /**
     * @brief Marks a todo item as done or undone.
     * 
     * The todo item with the specified ID is marked as done if the done parameter is true,
     * and marked as undone if the done parameter is false.
     */
    void markTodoDone(FileData *data, int id, bool done);
    /**
     * @brief Creates the todos component.
     * 
     * This function creates the todos component, which displays the list of todos and allows the user to interact with them.
     */
    ftxui::Component renderTodosComponent(FileData *data);

private:
    TodosListData todos;
    int selectedTodos = 0;
    std::string newTodoName = "";
    std::string newTodoDescription = "";

    std::string dueYear;
    std::string dueMonth;
    std::string dueDay;
    std::string dueHour;
    std::string dueMinute;
    // Private member variables and methods
};

#endif // TODOS_COMPONENT_H