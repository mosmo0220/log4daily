#include "../../Headers/Storage/localStorage.h"

#include <vector>
#include <string>
#include <fstream>
#include <json.hpp>

LocalStorage::LocalStorage(const std::string& configPath) {
    std::ifstream configFile(configPath);
    configFile >> configJson;
    configFile.close();
}

bool LocalStorage::createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName) {
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

FileData LocalStorage::openLog4DailyFile(const std::string& folderPath, std::string& name) {
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

bool LocalStorage::updateDataToFile(const std::string& folderPath, std::string& name, FileData& data) {
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

bool LocalStorage::deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName) {
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

bool LocalStorage::checkIfFileExists(const std::string& name) {
    std::vector<std::string> files = configJson["listOfFilesNames"];
    return std::find(files.begin(), files.end(), name) != files.end();
}

bool LocalStorage::saveConfig(const std::string& folderPath, const std::string& configName) {
    try {
        std::ofstream configFile(folderPath + configName);
        configFile << configJson.dump(4);
        configFile.close();
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}

bool LocalStorage::addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName) {
    if (!checkIfFileExists(name)) {
        configJson["listOfFilesNames"].push_back(name);
        return saveConfig(folderPath, configName);
    }
    return false;
}

bool LocalStorage::removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName) {
    if (checkIfFileExists(name)) {
        configJson["listOfFilesNames"].erase(std::remove(configJson["listOfFilesNames"].begin(), configJson["listOfFilesNames"].end(), name), configJson["listOfFilesNames"].end());
        return saveConfig(folderPath, configName);
    }
    return false;
}

nlohmann::json LocalStorage::DateToJson(const Date& date) {
    return {
        {"day", date.day},
        {"month", date.month},
        {"year", date.year},
        {"hour", date.hour},
        {"minute", date.minute}
    };
}

Date LocalStorage::JsonToDate(const nlohmann::json& j) {
    return {
        j["day"], j["month"], j["year"],
        j["hour"], j["minute"]
    };
}

nlohmann::json LocalStorage::TodoDataToJson(const TodoData& todo) {
    return {
        {"id", todo.id},
        {"createDate", DateToJson(todo.createDate)},
        {"dueDate", DateToJson(todo.dueDate)},
        {"todoName", todo.todoName},
        {"todoDescription", todo.todoDescription}
    };
}

TodoData LocalStorage::JsonToTodoData(const nlohmann::json& j) {
    return {
        j["id"],
        JsonToDate(j["createDate"]),
        JsonToDate(j["dueDate"]),
        j["todoName"],
        j["todoDescription"]
    };
}

nlohmann::json LocalStorage::MilestoneProgressPointToJson(const MilestoneProgressPoint& point) {
    return {
        {"date", DateToJson(point.date)},
        {"isCompleted", point.isCompleted}
    };
}

MilestoneProgressPoint LocalStorage::JsonToMilestoneProgressPoint(const nlohmann::json& j) {
    return {
        JsonToDate(j["date"]),
        j["isCompleted"]
    };
}

nlohmann::json LocalStorage::MilestonesDataToJson(const MilestonesData& milestone) {
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

MilestonesData LocalStorage::JsonToMilestonesData(const nlohmann::json& j) {
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

nlohmann::json LocalStorage::DiaryDataToJson(const DiaryData& diary) {
    return {
        {"id", diary.id},
        {"date", DateToJson(diary.date)},
        {"diaryEntryName", diary.diaryEntryName},
        {"diaryEntry", diary.diaryEntry}
    };
}

DiaryData LocalStorage::JsonToDiaryData(const nlohmann::json& j) {
    return {
        j["id"],
        JsonToDate(j["date"]),
        j["diaryEntryName"],
        j["diaryEntry"]
    };
}

nlohmann::json LocalStorage::FileDataToJson(const FileData& data) {
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

FileData LocalStorage::JsonToFileData(const nlohmann::json& j) {
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
