/**
 * @file localStorage.cpp
 * @brief Implementation of the LocalStorage class for managing log4daily files.
 * 
 * This file contains the implementation of the LocalStorage class, which provides
 * functionalities to create, open, update, and delete log4daily files. The data is
 * stored in JSON format using the nlohmann::json library.
 * 
 * The file also defines several data structures used to represent the data stored
 * in the log4daily files, including Date, TodoData, MilestoneProgressPoint, 
 * MilestonesData, DiaryData, and FileData.
 * 
 * The LocalStorage class provides the following public methods:
 * - LocalStorage(const std::string& configPath): Constructor that initializes the
 *   LocalStorage object with the configuration file.
 * - bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName):
 *   Creates a new log4daily file with the given name and configuration.
 * - FileData openLog4DailyFile(const std::string& folderPath, std::string& name):
 *   Opens an existing log4daily file and returns its data.
 * - bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data):
 *   Updates the data in an existing log4daily file.
 * - bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName):
 *   Deletes an existing log4daily file.
 * 
 * The class also provides several private helper methods for JSON serialization
 * and deserialization of the data structures, as well as methods for managing the
 * configuration file.
 * 
 * The following data structures are defined:
 * - struct Date: Represents a date and time.
 * - struct TodoData: Represents a to-do item.
 * - struct MilestoneProgressPoint: Represents a progress point in a milestone.
 * - struct MilestonesData: Represents a milestone with progress points.
 * - struct DiaryData: Represents a diary entry.
 * - struct FileData: Represents the data stored in a log4daily file.
 * 
 * The nlohmann::json library is used for JSON serialization and deserialization.
 */
#include <vector>
#include <string>

#include <fstream>
#include <json.hpp>

const std::string DUMB_CONFIG_FILE = "log4daily_config.json"; // For tests only
const std::string DUMB_LOG4DAILY_FOLDER = "./log4daily_files/"; // For tests only

/**
 * @struct Date
 * @brief Represents a date and time.
 * 
 * The Date structure represents a specific date and time, including the day, month, year,
 * hour, minute, and second components. It is used to store timestamps for various log4daily
 * data structures.
 * 
 * @var day The day component of the date.
 * @var month The month component of the date.
 * @var year The year component of the date.
 * @var hour The hour component of the time.
 * @var minute The minute component of the time.
 * @var second The second component of the time.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing Date structures.
 * @param other The Date structure to compare with.
 * @return bool True if the Date structures are equal, false otherwise.
 */
struct Date
{
    short day;
    short month;
    short year;

    short hour;
    short minute;
    short second;

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year && hour == other.hour && minute == other.minute && second == other.second;
    }
};

/**
 * @struct TodoData
 * @brief Represents a to-do item.
 * 
 * The TodoData structure represents a single to-do item, including an ID, creation date,
 * due date, name, and description. It is used to store to-do items in the log4daily files.
 * 
 * @var id The unique identifier of the to-do item.
 * @var createDate The date and time when the to-do item was created.
 * @var dueDate The due date and time for the to-do item.
 * @var todoName The name of the to-do item.
 * @var todoDescription The description of the to-do item.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing TodoData structures.
 * @param other The TodoData structure to compare with.
 * @return bool True if the TodoData structures are equal, false otherwise.
 */
struct TodoData
{
    int id;
    Date createDate;
    Date dueDate;
    std::string todoName;
    std::string todoDescription;

    bool operator==(const TodoData& other) const {
        return id == other.id && createDate == other.createDate && dueDate == other.dueDate && todoName == other.todoName && todoDescription == other.todoDescription;
    }
};

/**
 * @struct MilestoneProgressPoint
 * @brief Represents a progress point in a milestone.
 * 
 * The MilestoneProgressPoint structure represents a progress point in a milestone, including
 * a date, completion status, and progress description. It is used to track the progress of
 * milestones in the log4daily files.
 * 
 * @var date The date and time when the progress point was recorded.
 * @var isCompleted A flag indicating whether the milestone was completed at this point.
 * @var progressDescription A description of the progress made at this point.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing MilestoneProgressPoint structures.
 * @param other The MilestoneProgressPoint structure to compare with.
 * @return bool True if the MilestoneProgressPoint structures are equal, false otherwise.
 */
struct MilestoneProgressPoint 
{
    Date date;
    bool isCompleted;
    std::string progressDescription;

    bool operator==(const MilestoneProgressPoint& other) const {
        return date == other.date && isCompleted == other.isCompleted && progressDescription == other.progressDescription;
    }
};

/**
 * @struct MilestonesData
 * @brief Represents a milestone with progress points.
 * 
 * The MilestonesData structure represents a milestone, including an ID, start date, name,
 * description, and a list of progress points. It is used to store milestone data in the
 * log4daily files.
 * 
 * @var id The unique identifier of the milestone.
 * @var startDate The date and time when the milestone started.
 * @var milestoneName The name of the milestone.
 * @var milestoneDescription The description of the milestone.
 * @var progressPoints A vector of MilestoneProgressPoint structures representing the progress points of the milestone.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing MilestonesData structures.
 * @param other The MilestonesData structure to compare with.
 * @return bool True if the MilestonesData structures are equal, false otherwise.
 */
struct MilestonesData
{
    int id;
    Date startDate;
    std::string milestoneName;
    std::string milestoneDescription;
    std::vector<MilestoneProgressPoint> progressPoints;

    bool operator==(const MilestonesData& other) const {
        return id == other.id && startDate == other.startDate && milestoneName == other.milestoneName && milestoneDescription == other.milestoneDescription && progressPoints == other.progressPoints;
    }
};

/**
 * @struct DiaryData
 * @brief Represents a diary entry.
 * 
 * The DiaryData structure represents a diary entry, including an ID, date, entry name, and entry text.
 * It is used to store diary entries in the log4daily files.
 * 
 * @var id The unique identifier of the diary entry.
 * @var date The date and time when the diary entry was created.
 * @var diaryEntryName The name of the diary entry.
 * @var diaryEntry The text content of the diary entry.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing DiaryData structures.
 * @param other The DiaryData structure to compare with.
 * @return bool True if the DiaryData structures are equal, false otherwise.
 */
struct DiaryData
{
    int id;
    Date date;
    std::string diaryEntryName;
    std::string diaryEntry;

    bool operator==(const DiaryData& other) const {
        return id == other.id && date == other.date && diaryEntryName == other.diaryEntryName && diaryEntry == other.diaryEntry;
    }
};

/**
 * @struct FileData
 * @brief Represents the data stored in a log4daily file.
 * 
 * The FileData structure represents the data stored in a log4daily file, including the file name,
 * calendar data, to-do items, milestones, and diary entries. It is used to store and retrieve the
 * log4daily data from the files.
 * 
 * @var log4FileName The name of the log4daily file.
 * @var calendarData A vector of Date structures representing calendar events.
 * @var todosData A vector of TodoData structures representing to-do items.
 * @var milestonesData A vector of MilestonesData structures representing milestones.
 * @var diaryData A vector of DiaryData structures representing diary entries.
 * 
 * @fn operator==
 * @brief Overloaded equality operator for comparing FileData structures.
 * @param other The FileData structure to compare with.
 * @return bool True if the FileData structures are equal, false otherwise.
 */
struct FileData {
    std::string log4FileName;
    std::vector<Date> calendarData;
    std::vector<TodoData> todosData;
    std::vector<MilestonesData> milestonesData;
    std::vector<DiaryData> diaryData;

    bool operator==(const FileData& other) const {
        return log4FileName == other.log4FileName && calendarData == other.calendarData && todosData == other.todosData && milestonesData == other.milestonesData && diaryData == other.diaryData;
    }
};

/**
 * @class LocalStorage
 * @brief Provides functionalities to manage log4daily files.
 * 
 * The LocalStorage class provides functionalities to create, open, update, and delete log4daily files.
 * It stores the data in JSON format using the nlohmann::json library. The class also manages the configuration
 * file that stores the list of log4daily files.
 * 
 * The class also provides several private helper methods for JSON serialization and deserialization of the data structures,
 * as well as methods for managing the configuration file.
 */
class LocalStorage {
public:
    LocalStorage(const std::string& configPath) {
        std::ifstream configFile(configPath);
        configFile >> configJson;
        configFile.close();
    }

    /**
     * @brief Creates a new log4daily file with the given name and configuration.
     * 
     * This function creates a new log4daily file with the specified name and configuration.
     * If a file with the same name already exists, the function returns false without creating a new file.
     * The function also updates the configuration file to include the new file name.
     * 
     * @param folderPath The path to the folder where the log4daily files are stored.
     * @param name The name of the log4daily file to create.
     * @param configName The name of the configuration file.
     * @return bool True if the file was created successfully, false otherwise.
     */
    bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName) {
        if (checkIfFileExists(name)) {
            return false; // File already exists, no need to create it
        }
        std::string filePath = folderPath + name + ".json";
        std::ofstream outFile(filePath);
        if (!outFile.is_open()) {
            return false; // Failed to create the file
        }
        nlohmann::json emptyData = FileDataToJson(FileData({name, {}, {}, {}, {}}));
        outFile << emptyData.dump(4);
        outFile.close();

        addFileToConfig(name, folderPath, configName);
        return true;
    }

    /**
     * @brief Opens an existing log4daily file and returns its data.
     * 
     * This function opens an existing log4daily file with the specified name and returns its data.
     * If the file does not exist, the function returns an empty FileData object.
     * 
     * @param folderPath The path to the folder where the log4daily files are stored.
     * @param name The name of the log4daily file to open.
     * @return FileData The data stored in the log4daily file.
     */
    FileData openLog4DailyFile(const std::string& folderPath, std::string& name) {
        if (!checkIfFileExists(name)) {
            return FileData(); // Return an empty FileData object if the file does not exist
        }
        std::string filePath = folderPath + name + ".json";
        std::ifstream inFile(filePath);
        nlohmann::json jsonData;
        inFile >> jsonData;
        inFile.close();
        return JsonToFileData(jsonData);
    }

    /**
     * @brief Updates the data in an existing log4daily file.
     * 
     * This function updates the data in an existing log4daily file with the specified name.
     * If the file does not exist, the function returns false without updating the data.
     * 
     * @param folderPath The path to the folder where the log4daily files are stored.
     * @param name The name of the log4daily file to update.
     * @param data The new data to store in the log4daily file.
     * @return bool True if the data was updated successfully, false otherwise.
     */
    bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data) {
        if (!checkIfFileExists(name)) {
            return false; // File does not exist
        }
        try {
            std::string filePath = folderPath + name + ".json";
            std::ofstream outFile(filePath);
            nlohmann::json jsonData = FileDataToJson(data);
            outFile << jsonData.dump(4);
            outFile.close();
        } catch(const std::exception& e) {
            return false;
        }
        return true;
    }

    /**
     * @brief Deletes an existing log4daily file.
     * 
     * This function deletes an existing log4daily file with the specified name.
     * If the file does not exist, the function returns false without deleting any files.
     * The function also updates the configuration file to remove the deleted file name.
     * 
     * @param folderPath The path to the folder where the log4daily files are stored.
     * @param name The name of the log4daily file to delete.
     * @param configName The name of the configuration file.
     * @return bool True if the file was deleted successfully, false otherwise.
     */
    bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName) {
        if (!checkIfFileExists(name)) {
            return false; // File does not exist
        }
        std::string filePath = folderPath + name + ".json";
        if (std::remove(filePath.c_str()) != 0) {
            return false; // File could not be deleted
        } else {
            removeFileFromConfig(name, folderPath, configName);
            return true; // File has been deleted successfully
        }
    }

private:
    nlohmann::json configJson;

    bool checkIfFileExists(const std::string& name) {
        std::vector<std::string> files = configJson["listOfFilesNames"];
        return std::find(files.begin(), files.end(), name) != files.end();
    }

    bool saveConfig(const std::string& folderPath, const std::string& configName) {
        try {
            std::ofstream configFile(folderPath + configName);
            configFile << configJson.dump(4);
            configFile.close();
        } catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    bool addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName) {
        if (!checkIfFileExists(name)) {
            configJson["listOfFilesNames"].push_back(name);
            return saveConfig(folderPath, configName);
        }
        return false;
    }

    bool removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName) {
        if (checkIfFileExists(name)) {
            configJson["listOfFilesNames"].erase(std::remove(configJson["listOfFilesNames"].begin(), configJson["listOfFilesNames"].end(), name), configJson["listOfFilesNames"].end());
            return saveConfig(folderPath, configName);
        }
        return false;
    }

    nlohmann::json DateToJson(const Date& date) {
        return {
            {"day", date.day},
            {"month", date.month},
            {"year", date.year},
            {"hour", date.hour},
            {"minute", date.minute},
            {"second", date.second}
        };
    }

    Date JsonToDate(const nlohmann::json& j) {
        return {
            j["day"], j["month"], j["year"],
            j["hour"], j["minute"], j["second"]
        };
    }

    nlohmann::json TodoDataToJson(const TodoData& todo) {
        return {
            {"id", todo.id},
            {"createDate", DateToJson(todo.createDate)},
            {"dueDate", DateToJson(todo.dueDate)},
            {"todoName", todo.todoName},
            {"todoDescription", todo.todoDescription}
        };
    }

    TodoData JsonToTodoData(const nlohmann::json& j) {
        return {
            j["id"],
            JsonToDate(j["createDate"]),
            JsonToDate(j["dueDate"]),
            j["todoName"],
            j["todoDescription"]
        };
    }

    nlohmann::json MilestoneProgressPointToJson(const MilestoneProgressPoint& point) {
        return {
            {"date", DateToJson(point.date)},
            {"isCompleted", point.isCompleted},
            {"progressDescription", point.progressDescription}
        };
    }

    MilestoneProgressPoint JsonToMilestoneProgressPoint(const nlohmann::json& j) {
        return {
            JsonToDate(j["date"]),
            j["isCompleted"],
            j["progressDescription"]
        };
    }

    nlohmann::json MilestonesDataToJson(const MilestonesData& milestone) {
        nlohmann::json progressPointsJson = nlohmann::json::array();
        for (const auto& point : milestone.progressPoints) {
            progressPointsJson.push_back(MilestoneProgressPointToJson(point));
        }
        return {
            {"id", milestone.id},
            {"startDate", DateToJson(milestone.startDate)},
            {"milestoneName", milestone.milestoneName},
            {"milestoneDescription", milestone.milestoneDescription},
            {"progressPoints", progressPointsJson}
        };
    }

    MilestonesData JsonToMilestonesData(const nlohmann::json& j) {
        MilestonesData milestone;
        milestone.id = j["id"];
        milestone.startDate = JsonToDate(j["startDate"]);
        milestone.milestoneName = j["milestoneName"];
        milestone.milestoneDescription = j["milestoneDescription"];
        for (const auto& pointJson : j["progressPoints"]) {
            milestone.progressPoints.push_back(JsonToMilestoneProgressPoint(pointJson));
        }
        return milestone;
    }

    nlohmann::json DiaryDataToJson(const DiaryData& diary) {
        return {
            {"id", diary.id},
            {"date", DateToJson(diary.date)},
            {"diaryEntryName", diary.diaryEntryName},
            {"diaryEntry", diary.diaryEntry}
        };
    }

    DiaryData JsonToDiaryData(const nlohmann::json& j) {
        return {
            j["id"],
            JsonToDate(j["date"]),
            j["diaryEntryName"],
            j["diaryEntry"]
        };
    }

    nlohmann::json FileDataToJson(const FileData& data) {
        nlohmann::json j;
        j["log4FileName"] = data.log4FileName;

        j["calendarData"] = nlohmann::json::array();
        for (const auto& date : data.calendarData) {
            j["calendarData"].push_back(DateToJson(date));
        }

        j["todosData"] = nlohmann::json::array();
        for (const auto& todo : data.todosData) {
            j["todosData"].push_back(TodoDataToJson(todo));
        }

        j["milestonesData"] = nlohmann::json::array();
        for (const auto& milestone : data.milestonesData) {
            j["milestonesData"].push_back(MilestonesDataToJson(milestone));
        }

        j["diaryData"] = nlohmann::json::array();
        for (const auto& diary : data.diaryData) {
            j["diaryData"].push_back(DiaryDataToJson(diary));
        }

        return j;
    }

    FileData JsonToFileData(const nlohmann::json& j) {
        FileData data;
        data.log4FileName = j["log4FileName"];

        for (const auto& dateJson : j["calendarData"]) {
            data.calendarData.push_back(JsonToDate(dateJson));
        }

        for (const auto& todoJson : j["todosData"]) {
            data.todosData.push_back(JsonToTodoData(todoJson));
        }

        for (const auto& milestoneJson : j["milestonesData"]) {
            data.milestonesData.push_back(JsonToMilestonesData(milestoneJson));
        }

        for (const auto& diaryJson : j["diaryData"]) {
            data.diaryData.push_back(JsonToDiaryData(diaryJson));
        }

        return data;
    }
};
