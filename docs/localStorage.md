# LocalStorage Class Documentation

## Overview

The `LocalStorage` class provides an interface for managing data persistence in JSON files. It handles file operations such as creation, opening, updating, and deletion, as well as data serialization and deserialization between C++ objects and JSON format.

## Data Structures

The following data structures are used to represent the data stored in the JSON files:

* **`Date`:**  Represents a date and time with day, month, year, hour, minute, and second.
* **`TodoData`:** Represents a to-do item with an ID, creation date, due date, name, and description.
* **`MilestoneProgressPoint`:** Represents a progress point in a milestone with a date, completion status, and description.
* **`MilestonesData`:** Represents a milestone with an ID, start date, name, description, and a list of progress points.
* **`DiaryData`:** Represents a diary entry with an ID, date, name, and entry text.
* **`FileData`:**  Represents the overall data stored in a single file, including a file name, calendar data, to-do items, milestones, and diary entries.

Each data structure includes an overloaded `operator==` for equality comparison.

## Public Methods

### `LocalStorage(const std::string& configPath)`

* **Constructor:** Initializes the `LocalStorage` object by loading the configuration JSON file from the specified path.

### `bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`

* **Creates a new JSON file:** 
    * Checks if a file with the given name already exists. If it does, returns `false`.
    * Constructs the file path using the provided folder path and name.
    * Creates an empty JSON file at the specified path.
    * If the file creation is successful, adds the file name to the configuration JSON and returns `true`. Otherwise, returns `false`.

### `FileData openLog4DailyFile(const std::string& folderPath, std::string& name)`

* **Opens and reads an existing JSON file:**
    * Checks if a file with the given name exists. If it doesn't, returns an empty `FileData` object.
    * Constructs the file path using the provided folder path and name.
    * Opens the JSON file and reads its contents.
    * Deserializes the JSON data into a `FileData` object and returns it.

### `bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data)`

* **Updates an existing JSON file with new data:**
    * Checks if a file with the given name exists. If it doesn't, returns `false`.
    * Constructs the file path using the provided folder path and name.
    * Opens the JSON file and writes the serialized `FileData` object to it.
    * Returns `true` if the update is successful, `false` otherwise.

### `bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`

* **Deletes an existing JSON file:**
    * Checks if a file with the given name exists. If it doesn't, returns `false`.
    * Constructs the file path using the provided folder path and name.
    * Deletes the JSON file.
    * If the file deletion is successful, removes the file name from the configuration JSON and returns `true`. Otherwise, returns `false`.

## Private Methods

### `bool checkIfFileExists(const std::string& name)`

* **Checks if a file with the given name exists in the configuration JSON.**

### `bool saveConfig(const std::string& folderPath, const std::string& configName)`

* **Saves the configuration JSON to a file.**

### `bool addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName)`

* **Adds a file name to the list of files in the configuration JSON.**

### `bool removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName)`

* **Removes a file name from the list of files in the configuration JSON.**

### `nlohmann::json DateToJson(const Date& date)`

* **Serializes a `Date` object to a JSON object.**

### `Date JsonToDate(const nlohmann::json& j)`

* **Deserializes a JSON object to a `Date` object.**

### `nlohmann::json TodoDataToJson(const TodoData& todo)`

* **Serializes a `TodoData` object to a JSON object.**

### `TodoData JsonToTodoData(const nlohmann::json& j)`

* **Deserializes a JSON object to a `TodoData` object.**

### `nlohmann::json MilestoneProgressPointToJson(const MilestoneProgressPoint& point)`

* **Serializes a `MilestoneProgressPoint` object to a JSON object.**

### `MilestoneProgressPoint JsonToMilestoneProgressPoint(const nlohmann::json& j)`

* **Deserializes a JSON object to a `MilestoneProgressPoint` object.**

### `nlohmann::json MilestonesDataToJson(const MilestonesData& milestone)`

* **Serializes a `MilestonesData` object to a JSON object.**

### `MilestonesData JsonToMilestonesData(const nlohmann::json& j)`

* **Deserializes a JSON object to a `MilestonesData` object.**

### `nlohmann::json DiaryDataToJson(const DiaryData& diary)`

* **Serializes a `DiaryData` object to a JSON object.**

### `DiaryData JsonToDiaryData(const nlohmann::json& j)`

* **Deserializes a JSON object to a `DiaryData` object.**

### `nlohmann::json FileDataToJson(const FileData& data)`

* **Serializes a `FileData` object to a JSON object.**

### `FileData JsonToFileData(const nlohmann::json& j)`

* **Deserializes a JSON object to a `FileData` object.**


## Constants

* `DUMB_CONFIG_FILE`:  (For tests only)  Default name for the configuration file.
* `DUMB_LOG4DAILY_FOLDER`: (For tests only) Default folder path for storing data files.

## Dependencies

* `nlohmann::json` library for JSON handling.
* `fstream` for file input/output operations.
* `vector` for dynamic arrays.
* `string` for string manipulation.