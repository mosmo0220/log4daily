#ifndef HANDLERS_H
#define HANDLERS_H

#include <vector>
#include <string>
#include <algorithm>

struct Command {
    std::string name;
    std::string argument;

    bool operator==(const Command& other) const {
        return name == other.name && argument == other.argument;
    }
};

static const std::vector<std::string> supportedCommands = {
    "--help",
    "--new",
    "--open",
    "--delete",
    // Will be implemented in the future
    "--list",
    "--import",
    "--export",
    "--set-theme",
    "--set-language"   
};

class InputHandlers {
    public:
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
        static std::vector<Command> parseConsoleInputs(int argc, char* argv[]);
        /**
         * @brief Filters a vector of Command structures for supported commands.
         * 
         * This function takes a vector of Command structures and filters out any commands that are not supported.
         * The supported commands are defined in the supportedCommands vector.
         * 
         * @param commands The vector of Command structures to filter.
         * @return std::vector<Command> A vector of Command structures containing only supported commands.
        */
        static std::vector<Command> filterForSupportedCommands(const std::vector<Command>& commands);
};

#endif // HANDLERS_H