**RenderUI**

File: renderUI.cpp

Description: The `RenderUI` class is responsible for rendering the user interface of the log4daily application.

* Public methods:
    * `RenderUI(MainThread _mainThread)`: Constructor that initializes `RenderUI` with a `MainThread` object.
    * `Component createExitComponent(ScreenInteractive& screen, int& exitSelected)`: Creates an exit component with options to save and exit or exit without saving.
    * `int renderUI()`: Renders UI components and handles user interaction.
    * `std::string getExitMessage()`: Returns the exit message (with or without saving).

* Attributes:
    * `mainThread`: `MainThread` object to manage the main application thread.
    * `inUseFileData`: Log4daily file data used in the UI.
    * `exitedWithoutSaving`: Flag indicating whether the user exited without saving changes.