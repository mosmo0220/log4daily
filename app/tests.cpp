#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

#include "handlers.cpp"
#include <vector>
#include <string>

#include "localStorage.cpp"

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

// Test suite for the localStorage
TEST_SUITE("localStorage") {
    TEST_CASE("createLog4DailyFile") {
        // Test input
        std::string folderPath = DUMB_LOG4DAILY_FOLDER;
        std::string configName = DUMB_CONFIG_FILE;
        std::string name = "test_file";

        LocalStorage localStorage(folderPath + configName);
        bool result = localStorage.createLog4DailyFile(folderPath, name, configName);

        // Expected output
        CHECK_EQ(result, true);
    }

    TEST_CASE("openLog4DailyFile") {
        // Test input
        std::string folderPath = DUMB_LOG4DAILY_FOLDER;
        std::string configName = DUMB_CONFIG_FILE;
        std::string name = "test_file";

        LocalStorage localStorage(folderPath + configName);
        FileData fileData = localStorage.openLog4DailyFile(folderPath, name);
        FileData expected = FileData();

        // Expected output
        CHECK_EQ(fileData, expected);
    }

    TEST_CASE("updateDataToFile") {
        // Test input
        std::string folderPath = DUMB_LOG4DAILY_FOLDER;
        std::string configName = DUMB_CONFIG_FILE;
        std::string name = "test_file";

        LocalStorage localStorage(folderPath + configName);
        FileData fileData = localStorage.openLog4DailyFile(folderPath, name);
        fileData.log4FileName = "updated_test_file";
        bool result = localStorage.updateDataToFile(folderPath, name, fileData);

        // Expected output
        CHECK_EQ(result, true);
    }

    TEST_CASE("deleteLog4DailyFile") {
        // Test input
        std::string folderPath = DUMB_LOG4DAILY_FOLDER;
        std::string configName = DUMB_CONFIG_FILE;
        std::string name = "test_file";

        LocalStorage localStorage(folderPath + configName);
        bool result = localStorage.deleteLog4DailyFile(folderPath, name, configName);

        // Expected output
        CHECK_EQ(result, true);
    }
}