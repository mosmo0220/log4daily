#ifndef MANAGECONFIG_H
#define MANAGECONFIG_H

#include <string>
#include <vector>
#include <json.hpp>

enum ConfigFolderStatus {
    FOLDER_CREATED,
    FOLDER_EXISTS,
    FOLDER_ERROR
};

struct SystemConfiguration {
	std::string system;
	std::string systemLanguage;
	std::string configPath;
	std::string systemTheme;
};

enum class SupportedLanguages {
	English,
	Polish
};

enum class Themes {
	Light,
	Dark
};

struct Settings {
	std::vector<std::string> listOfFilesNames;
	SupportedLanguages selectedLanguage;
	Themes selectedTheme;
	SystemConfiguration readedSystemConfiguration;
};

class ManageConfig {
public:
    std::string configFileName = "settings.json";
    ManageConfig() {};
    /**
     * @brief Manages the configuration for the log4daily application.
     * 
     * This function manages the configuration for the log4daily application by creating the configuration folder
     * and configuration file if they do not already exist.
     * 
     * @return std::string The path to the configuration folder.
     */
    ConfigFolderStatus prepareConfigFile();
    /**
     * @brief Gets the path to the configuration folder, os name and language.
     * 
     * This function gets the path to the configuration folder, os name and language for the log4daily application.
     */
    SystemConfiguration getSystemConfiguration();
    /**
     * @brief Creates the configuration folder.
     * 
     * This function creates the configuration folder for the log4daily application.
     */
    ConfigFolderStatus createConfigFolder(const std::string& path);
	
    nlohmann::json SystemConfigurationToJson(const SystemConfiguration& config);
	SystemConfiguration JsonToSystemConfiguration(const nlohmann::json& j);
	nlohmann::json SupportedLanguagesToJson(const SupportedLanguages& lang);
	SupportedLanguages JsonToSupportedLanguages(const nlohmann::json& j);
	nlohmann::json ThemesToJson(const Themes& theme);
	Themes JsonToThemes(const nlohmann::json& j);
	nlohmann::json SettingsToJson(const Settings& settings);
	Settings JsonToSettings(const nlohmann::json& j);
};

#endif // MANAGECONFIG_H