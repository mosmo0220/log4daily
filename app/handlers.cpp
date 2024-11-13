#include "handlers.h"

#include <vector>
#include <string>
#include <algorithm>

std::vector<Command> Handlers::parseConsoleInputs(int argc, char* argv[]) {
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

std::vector<Command> Handlers::filterForSupportedCommands(const std::vector<Command>& commands) {
    std::vector<Command> filteredCommands;

    for (const auto& cmd : commands) {
        if (std::find(supportedCommands.begin(), supportedCommands.end(), cmd.name) != supportedCommands.end()) {
            filteredCommands.push_back(cmd);
        }
    }

    return filteredCommands;
}
