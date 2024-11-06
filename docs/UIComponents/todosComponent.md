**todosComponent**

File: todosComponent.cpp

Description: The `todosComponent` function creates the "Todos" component, which displays a list of to-dos and allows the user to interact with it.

* Functions:
    * `limitedInput(std::string* content, const std::string& placeholder, int maxLength)`: Creates an input component with a limited length for the entered text.
    * `addTodo(FileData *data, std::string newTodoName, std::string newTodoDescription, Date dueDate)`: Adds a new to-do item to the list.
    * `removeTodo(FileData *data, int id)`: Removes a to-do item from the list based on its ID.
    * `markTodoDone(FileData *data, int id, bool done)`: Marks a to-do item as done or undone.

* Global variables:
    * `todos`: A `TodosListData` structure storing the list of to-dos and their IDs.
    * `selectedTodos`: Index of the currently selected to-do item on the list.
    * `newTodoName`: Name of the new to-do item.
    * `newTodoDescription`: Description of the new to-do item.
    * `dueYear`, `dueMonth`, `dueDay`, `dueHour`, `dueMinute`: Variables storing the year, month, day, hour, and minute of the to-do item's due date.

* UI elements:
    * To-do list (`todosList`) with the ability to select items.
    * Input fields for entering the name (`newTodoInput`) and description (`descriptionInput`) of a new to-do item.
    * Input fields for entering the due date of a to-do item (`dueDateInputs`).
    * Buttons for adding (`addButton`), removing (`removeButton`), and marking as done (`markDoneButton`) to-do items.
    * Display of information about the selected to-do item: due date (`selectedTodoDate`) and description (`selectedTodoDescription`).