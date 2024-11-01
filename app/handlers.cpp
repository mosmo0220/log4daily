/**
 * @file handlers.cpp
 * @brief This file contains functions for parsing and filtering console commands.
 * 
 * The file defines two namespaces: `consoleHandlers` and `controlHandlers`. 
 * The `consoleHandlers` namespace includes a `Command` structure and a function to parse console inputs into a vector of `Command` structures.
 * The `controlHandlers` namespace includes a function to filter a vector of `Command` structures for supported commands.
 * 
 * @namespace consoleHandlers
 * The `consoleHandlers` namespace contains structures and functions for handling console commands.
 * 
 * @struct consoleHandlers::Command
 * @brief Represents a console command with a name and an argument.
 * 
 * @fn consoleHandlers::parseConsoleInputs
 * @brief Parses console inputs into a vector of `Command` structures.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return std::vector<consoleHandlers::Command> A vector of parsed `Command` structures.
 * 
 * @namespace controlHandlers
 * The `controlHandlers` namespace contains functions for filtering console commands.
 * 
 * @var controlHandlers::supportedCommands
 * @brief A vector of strings representing the supported commands.
 * 
 * @fn controlHandlers::filterForSupportedCommands
 * @brief Filters a vector of `Command` structures for supported commands.
 * @param commands The vector of `Command` structures to filter.
 * @return std::vector<consoleHandlers::Command> A vector of `Command` structures containing only supported commands.
 */
#include <vector>
#include <string>
#include <algorithm>

namespace consoleHandlers {
    struct Command {
        std::string name;
        std::string argument;

        bool operator==(const Command& other) const {
            return name == other.name && argument == other.argument;
        }
    };

    /**
     * @brief Parses console inputs into a vector of Command structures.
     * 
     * This function takes the command-line arguments and parses them into a vector of Command structures.
     * Each Command structure contains a command name and a vector of arguments associated with that command.
     * 
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line arguments.
     * @return std::vector<Command> A vector of parsed Command structures.
    */
    std::vector<Command> parseConsoleInputs(int argc, char* argv[]) {
        std::vector<Command> commands;

        for (int i = 0; i < argc; ++i) {
            if (argv[i][0] == '-' && argv[i][1] == '-') {
                Command cmd;
                cmd.name = argv[i];

                bool hasArgument = false;

                while (i + 1 < argc && argv[i + 1][0] != '-') {
                    i++;
                    if (!hasArgument) {
                        hasArgument = true;
                        cmd.argument = argv[i];
                    }
                }

                commands.push_back(cmd);
            }
        }

        return commands;
    }
}

namespace controlHandlers { 
    static const std::vector <std::string> supportedCommands = {
        "--help",
        "--new",
        "--open",
        "--delete",
    };

    /**
     * @brief Filters a vector of Command structures for supported commands.
     * 
     * This function takes a vector of Command structures and filters out any commands that are not supported.
     * The supported commands are defined in the supportedCommands vector.
     * 
     * @param commands The vector of Command structures to filter.
     * @return std::vector<Command> A vector of Command structures containing only supported commands.
    */
    std::vector<consoleHandlers::Command> filterForSupportedCommands(const std::vector<consoleHandlers::Command>& commands) {
        std::vector<consoleHandlers::Command> supportedCommands;

        for (const auto& cmd : commands) {
            if (std::find(controlHandlers::supportedCommands.begin(), controlHandlers::supportedCommands.end(), cmd.name) != controlHandlers::supportedCommands.end()) {
                supportedCommands.push_back(cmd);
            }
        }

        return supportedCommands;
    }
}
