**MainThread**

File: mainThread.cpp

Description: The `MainThread` class manages the main thread of the log4daily application. It handles parsing console input, filtering supported commands, and executing them.

* Public methods:
    * `MainThread(std::string workingDirectory, std::string configPath)`: Constructor that initializes `MainThread` with the specified working directory and configuration file path.
    * `CommandType run(int argc, char* argv[])`: Parses and executes commands from command-line arguments.
    * `void showMessage(CommandType respond, std::string message = "")`: Displays a message in the console based on the result of command execution.
    * `std::string getRespondMessage()`: Returns the response message after command execution.
    * `FileData getOpenedFile()`: Returns the data of the opened log4daily file.

* Private methods:
    * `void showHelp()`: Displays help for available commands.

* Attributes:
    * `localStorage`: `LocalStorage` object to manage log4daily files.
    * `workingDirectory`: Working directory of the application.
    * `configName`: Name of the configuration file.
    * `openedFile`: Data of the opened log4daily file.
    * `respondMessage`: Response message after command execution.

* Enum type `CommandType`:
    * Defines the types of commands that can be executed by the application: `New`, `Open`, `Delete`, `Help`, `Unsupported`, `Failed`, `Other`.