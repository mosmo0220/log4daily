## MainThread Class Documentation

### Overview

The `MainThread` class manages the main thread of the log4daily application. It handles command-line arguments, filters supported commands, and executes them. It also provides methods for displaying help messages and responding to user inputs.

### Public Methods

#### `MainThread(std::string workingDirectory, std::string configPath)`

* **Constructor:** Initializes the `MainThread` object with the working directory and configuration file path.

#### `CommandType run(int argc, char* argv[])`

* **Parses and executes commands:**
    * Parses the command-line arguments using `consoleHandlers::parseConsoleInputs`.
    * Filters the parsed commands for supported commands using `controlHandlers::filterForSupportedCommands`.
    * Executes the first supported command.
    * Returns a `CommandType` enum value indicating the result of the command execution.

#### `void ShowMessage(CommandType respond, std::string message = "")`

* **Displays a message:**
    * Prints a message to the console based on the `CommandType` enum value.
    * Optionally displays an additional message.

#### `std::string getRespondMessage()`

* **Returns the response message:**
    * Retrieves the response message generated after command execution.

#### `FileData getOpenedFile()`

* **Returns the opened file data:**
    * Retrieves the data of the opened log4daily file.


### Private Methods

#### `void show_help()`

* **Displays the help message:**
    * Prints the usage instructions and a list of available commands to the console.

### Enums

#### `CommandType`

* **Represents the result of a command execution:**
    * `NEW`: A new log4daily file was created.
    * `OPEN`: An existing log4daily file was opened.
    * `DELETE`: An existing log4daily file was deleted.
    * `HELP`: The help message was displayed.
    * `UNSUPPORTED`: No supported command was found.
    * `FAILED`: The command execution failed.
    * `OTHER`: Other command execution results.

## Dependencies

* `LocalStorage` class for handling log4daily files.
* `consoleHandlers` module for parsing console inputs.
* `controlHandlers` module for filtering supported commands.
* `iostream` for standard input/output operations.
* `string` for string manipulation.
* `vector` for dynamic arrays.


## Example Usage

```cpp
MainThread mt("/path/to/working/directory", "config.cfg");
CommandType result = mt.run(argc, argv);
mt.ShowMessage(result);
```

## Note

This class relies on external dependencies such as `LocalStorage`, `consoleHandlers`, and `controlHandlers`, which are assumed to be defined elsewhere in the project.
