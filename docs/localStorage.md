**LocalStorage**

File: localStorage.cpp

Description: The `LocalStorage` class provides functionalities to manage log4daily files. It stores data in JSON format and manages the configuration file that stores the list of log4daily files.

* Data structures:
    * `Date`: Stores date and time.
    * `TodoData`: Stores information about a to-do item.
    * `MilestoneProgressPoint`: Stores information about a milestone progress point.
    * `MilestonesData`: Stores information about a milestone.
    * `DiaryData`: Stores a diary entry.
    * `FileData`: Stores all data in a log4daily file.

* Public methods:
    * `LocalStorage(const std::string& configPath)`: Constructor that loads the configuration file.
    * `bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`: Creates a new log4daily file.
    * `FileData openLog4DailyFile(const std::string& folderPath, std::string& name)`: Opens an existing log4daily file.
    * `bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data)`: Updates data in a log4daily file.
    * `bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`: Deletes a log4daily file.

* Private methods:
    * `bool checkIfFileExists(const std::string& name)`: Checks if a file exists.
    * `bool saveConfig(const std::string& folderPath, const std::string& configName)`: Saves the configuration file.
    * `bool addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName)`: Adds a file to the configuration file.
    * `bool removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName)`: Removes a file from the configuration file.
    * `nlohmann::json DateToJson(const Date& date)`: Converts `Date` to JSON.
    * `Date JsonToDate(const nlohmann::json& j)`: Converts JSON to `Date`.
    * `nlohmann::json TodoDataToJson(const TodoData& todo)`: Converts `TodoData` to JSON.
    * `TodoData JsonToTodoData(const nlohmann::json& j)`: Converts JSON to `TodoData`.
    * `nlohmann::json MilestoneProgressPointToJson(const MilestoneProgressPoint& point)`: Converts `MilestoneProgressPoint` to JSON.
    * `MilestoneProgressPoint JsonToMilestoneProgressPoint(const nlohmann::json& j)`: Converts JSON to `MilestoneProgressPoint`.
    * `nlohmann::json MilestonesDataToJson(const MilestonesData& milestone)`: Converts `MilestonesData` to JSON.
    * `MilestonesData JsonToMilestonesData(const nlohmann::json& j)`: Converts JSON to `MilestonesData`.
    * `nlohmann::json DiaryDataToJson(const DiaryData& diary)`: Converts `DiaryData` to JSON.
    * `DiaryData JsonToDiaryData(const nlohmann::json& j)`: Converts JSON to `DiaryData`.
    * `nlohmann::json FileDataToJson(const FileData& data)`: Converts `FileData` to JSON.
    * `FileData JsonToFileData(const nlohmann::json& j)`: Converts JSON to `FileData`.

* Dependencies:
    * `nlohmann::json` library for JSON handling.
    * `fstream` for file input/output operations.
    * `vector` for dynamic arrays.
    * `string` for string manipulation.