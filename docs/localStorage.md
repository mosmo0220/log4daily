**LocalStorage**

Plik: localStorage.cpp

Opis: Klasa `LocalStorage` zapewnia funkcjonalności do zarządzania plikami log4daily. Przechowuje dane w formacie JSON i zarządza plikiem konfiguracyjnym, który przechowuje listę plików log4daily.

* Struktury danych:
    * `Date`: Przechowuje datę i czas.
    * `TodoData`: Przechowuje informacje o zadaniu do wykonania.
    * `MilestoneProgressPoint`: Przechowuje informacje o punkcie kontrolnym w kamieniu milowym.
    * `MilestonesData`: Przechowuje informacje o kamieniu milowym.
    * `DiaryData`: Przechowuje wpis do dziennika.
    * `FileData`: Przechowuje wszystkie dane w pliku log4daily.

* Metody publiczne:
    * `LocalStorage(const std::string& configPath)`: Konstruktor, który ładuje plik konfiguracyjny.
    * `bool createLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`: Tworzy nowy plik log4daily.
    * `FileData openLog4DailyFile(const std::string& folderPath, std::string& name)`: Otwiera istniejący plik log4daily.
    * `bool updateDataToFile(const std::string& folderPath, std::string& name, FileData& data)`: Aktualizuje dane w pliku log4daily.
    * `bool deleteLog4DailyFile(const std::string& folderPath, std::string& name, const std::string& configName)`: Usuwa plik log4daily.

* Metody prywatne:
    * `bool checkIfFileExists(const std::string& name)`: Sprawdza, czy plik istnieje.
    * `bool saveConfig(const std::string& folderPath, const std::string& configName)`: Zapisuje plik konfiguracyjny.
    * `bool addFileToConfig(std::string& name, const std::string& folderPath, const std::string& configName)`: Dodaje plik do pliku konfiguracyjnego.
    * `bool removeFileFromConfig(std::string& name, const std::string& folderPath, const std::string& configName)`: Usuwa plik z pliku konfiguracyjnego.
    * `nlohmann::json DateToJson(const Date& date)`: Konwertuje `Date` na JSON.
    * `Date JsonToDate(const nlohmann::json& j)`: Konwertuje JSON na `Date`.
    * `nlohmann::json TodoDataToJson(const TodoData& todo)`: Konwertuje `TodoData` na JSON.
    * `TodoData JsonToTodoData(const nlohmann::json& j)`: Konwertuje JSON na `TodoData`.
    * `nlohmann::json MilestoneProgressPointToJson(const MilestoneProgressPoint& point)`: Konwertuje `MilestoneProgressPoint` na JSON.
    * `MilestoneProgressPoint JsonToMilestoneProgressPoint(const nlohmann::json& j)`: Konwertuje JSON na `MilestoneProgressPoint`.
    * `nlohmann::json MilestonesDataToJson(const MilestonesData& milestone)`: Konwertuje `MilestonesData` na JSON.
    * `MilestonesData JsonToMilestonesData(const nlohmann::json& j)`: Konwertuje JSON na `MilestonesData`.
    * `nlohmann::json DiaryDataToJson(const DiaryData& diary)`: Konwertuje `DiaryData` na JSON.
    * `DiaryData JsonToDiaryData(const nlohmann::json& j)`: Konwertuje JSON na `DiaryData`.
    * `nlohmann::json FileDataToJson(const FileData& data)`: Konwertuje `FileData` na JSON.
    * `FileData JsonToFileData(const nlohmann::json& j)`: Konwertuje JSON na `FileData`.

* Stałe:
    * `DUMB_CONFIG_FILE`: Nazwa pliku konfiguracyjnego (tylko do testów).
    * `DUMB_LOG4DAILY_FOLDER`: Ścieżka do folderu z plikami log4daily (tylko do testów).

* Zależności:
    * Biblioteka `nlohmann::json` do obsługi JSON.
    * `fstream` do operacji wejścia/wyjścia plików.
    * `vector` dla dynamicznych tablic.
    * `string` do obsługi ciągów znaków.
