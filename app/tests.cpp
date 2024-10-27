#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

#include "handlers.cpp"
#include <vector>
#include <string>

// Test suite for the consoleHandlers namespace
TEST_SUITE("consoleHandlers") {
    TEST_CASE("parseConsoleInputs") {
        // Test input
        int argc = 5;
        char* argv[] = {
            (char*)"--help",
            (char*)"--new",
            (char*)"arg1",
            (char*)"arg2",
            (char*)"--open",
        };

        auto commands = consoleHandlers::parseConsoleInputs(argc, argv);

        // Expected output
        std::vector<consoleHandlers::Command> expected = {
            {"--help", ""},
            {"--new", "arg1"},
            {"--open", ""},
        };

        CHECK_EQ(commands, expected);
    }
}

// Test suite for the controlHandlers namespace
TEST_SUITE("controlHandlers") {
    TEST_CASE("filterForSupportedCommands") {
        // Test input
        std::vector<consoleHandlers::Command> commands = {
            {"--help", {}},
            {"--new", {"arg1", "arg2"}},
            {"--open", {}},
            {"--delete", {"arg3"}},
            {"--invalid", {}},
        };

        auto supportedCommands = controlHandlers::filterForSupportedCommands(commands);

        // Expected output
        std::vector<consoleHandlers::Command> expected = {
            {"--help", {}},
            {"--new", {"arg1", "arg2"}},
            {"--open", {}},
            {"--delete", {"arg3"}},
        };

        CHECK_EQ(supportedCommands, expected);
    }
}
