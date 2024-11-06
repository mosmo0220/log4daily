**handlers**

File: handlers.cpp

Description: The `consoleHandlers` and `controlHandlers` namespaces contain functions for parsing and filtering console commands.

* `consoleHandlers`:
    * `struct Command`: Structure representing a command with a name and an argument.
    * `std::vector<Command> parseConsoleInputs(int argc, char* argv[])`: Parses command-line arguments into a vector of `Command` structures.

* `controlHandlers`:
    * `static const std::vector<std::string> supportedCommands`: Vector of supported commands.
    * `std::vector<consoleHandlers::Command> filterForSupportedCommands(const std::vector<consoleHandlers::Command>& commands)`: Filters a vector of commands, returning only the supported ones.
