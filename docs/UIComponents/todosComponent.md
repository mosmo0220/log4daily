**todosComponent**

Plik: todosComponent.cpp

Opis: Funkcja `todosComponent` tworzy komponent "Todos", który wyświetla listę zadań i pozwala użytkownikowi na interakcję z nią. 

* Funkcje:
    * `limitedInput(std::string* content, const std::string& placeholder, int maxLength)`: Tworzy komponent input z ograniczeniem długości wprowadzanego tekstu.
    * `addTodo(FileData *data, std::string newTodoName, std::string newTodoDescription, Date dueDate)`: Dodaje nowe zadanie do listy.
    * `removeTodo(FileData *data, int id)`: Usuwa zadanie z listy na podstawie jego ID.
    * `markTodoDone(FileData *data, int id, bool done)`: Oznacza zadanie jako wykonane lub niewykonane.

* Zmienne globalne:
    * `todos`: Struktura `TodosListData` przechowująca listę zadań i ich ID.
    * `selectedTodos`: Indeks aktualnie wybranego zadania na liście.
    * `newTodoName`: Nazwa nowego zadania.
    * `newTodoDescription`: Opis nowego zadania.
    * `dueYear`, `dueMonth`, `dueDay`, `dueHour`, `dueMinute`: Zmienne przechowujące rok, miesiąc, dzień, godzinę i minutę terminu wykonania zadania.

* Elementy interfejsu:
    * Lista zadań (`todosList`) z możliwością wyboru.
    * Pola do wprowadzania nazwy (`newTodoInput`) i opisu (`descriptionInput`) nowego zadania.
    * Pola do wprowadzania terminu wykonania zadania (`dueDateInputs`).
    * Przyciski do dodawania (`addButton`), usuwania (`removeButton`) i oznaczania jako wykonane (`markDoneButton`) zadań.
    * Wyświetlanie informacji o wybranym zadaniu: termin wykonania (`selectedTodoDate`) i opis (`selectedTodoDescription`).
