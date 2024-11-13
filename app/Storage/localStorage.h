/**
 * @file localStorage.h
 * 
 * @brief This file contains the declarations for the LocalStorage class and related data structures.
 * 
 * The LocalStorage class provides functionality for creating, opening, updating, and deleting log4daily files.
 */
#ifndef LOCAL_STORAGE_H
#define LOCAL_STORAGE_H

#include <string>
#include <vector>

#include <fstream>
#include <json.hpp>

/**
 * @struct Date
 * 
 * @brief Represents a date and time.
 * 
 * The Date structure represents a date and time, including the day, month, year, hour, minute, and second.
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
 * 
 * @brief Represents a to-do item.
 * 
 * The TodoData structure represents a to-do item, including an ID, creation date, due date, name, and description.
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
 * 
 * @brief Represents a milestone progress point.
 * 
 * The MilestoneProgressPoint structure represents a milestone progress point, including a date, completion status, and description.
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
 * 
 * @brief Represents a milestone.
 * 
 * The MilestonesData structure represents a milestone, including an ID, start date, name, description, and progress points.
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
 * 
 * @brief Represents a diary entry.
 * 
 * The DiaryData structure represents a diary entry, including an ID, date, name, and entry text.
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

class LocalStorage {
public:
    LocalStorage(const std::string& configPath);
    /**
     * @brief Creates a new log4daily file.
     * 
     * This function creates a new log4daily file with the specified name and configuration.
     */
    bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName);
    /**
     * @brief Opens an existing log4daily file.
     * 
     * This function opens an existing log4daily file with the specified name and returns its data.
     */
    FileData openLog4DailyFile(const std::string& folderPath, std::string& name);
    /**
     * @brief Updates the data in a log4daily file.
     * 
     * This function updates the data in an existing log4daily file with the specified name.
     */
    bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data);
    /**
     * @brief Deletes a log4daily file.
     * 
     * This function deletes an existing log4daily file with the specified name.
     */
    bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName);

private:
    nlohmann::json configJson;

    /**
     * @brief Checks if a file exists in the configuration.
     * 
     * This function checks if a file with the specified name exists in the configuration.
     */
    bool checkIfFileExists(const std::string& name);
    /**
     * @brief Saves the configuration to a file.
     * 
     * This function saves the configuration to a file with the specified name.
     */
    bool saveConfig(const std::string& folderPath, const std::string& configName);
    /**
     * @brief Adds a file to the configuration.
     * 
     * This function adds a file with the specified name to the configuration.
     */
    bool addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName);
    /**
     * @brief Removes a file from the configuration.
     * 
     * This function removes a file with the specified name from the configuration.
     */
    bool removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName);

    nlohmann::json DateToJson(const Date& date);
    Date JsonToDate(const nlohmann::json& j);
    nlohmann::json TodoDataToJson(const TodoData& todo);
    TodoData JsonToTodoData(const nlohmann::json& j);
    nlohmann::json MilestoneProgressPointToJson(const MilestoneProgressPoint& point);
    MilestoneProgressPoint JsonToMilestoneProgressPoint(const nlohmann::json& j);
    nlohmann::json MilestonesDataToJson(const MilestonesData& milestone);
    MilestonesData JsonToMilestonesData(const nlohmann::json& j);
    nlohmann::json DiaryDataToJson(const DiaryData& diary);
    DiaryData JsonToDiaryData(const nlohmann::json& j);
    nlohmann::json FileDataToJson(const FileData& data);
    FileData JsonToFileData(const nlohmann::json& j);
};

#endif // LOCAL_STORAGE_H
