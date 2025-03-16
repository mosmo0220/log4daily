#include "../../Headers/Storage/manageConfig.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>

#include <json.hpp>

#ifdef _WIN32
#include <direct.h>
#include <shlobj.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

SystemConfiguration ManageConfig::getSystemConfiguration() {
    std::string configPath;
    std::string osName;
    std::string language;
	std::string theme;

#ifdef _WIN32
    osName = "Windows";

    wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
    if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) {
        char localeNameA[LOCALE_NAME_MAX_LENGTH];
        size_t converted = 0;
        wcstombs_s(&converted, localeNameA, localeName, LOCALE_NAME_MAX_LENGTH);
        language = std::string(localeNameA);
    }
    else {
        language = "Nieznany";
    }

    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path))) {
        configPath = std::string(path) + "\\Log4Daily";
    } else {
        std::cerr << "Error getting user documents folder path" << std::endl;
        exit(1);
    }

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value;
        DWORD valueSize = sizeof(value);
        if (RegQueryValueEx(hKey, "AppsUseLightTheme", NULL, NULL, (LPBYTE)&value, &valueSize) == ERROR_SUCCESS) {
            theme = (value == 0) ? "Dark" : "Light";
        }
        else {
            theme = "Unknown";
        }
        RegCloseKey(hKey);
    }
    else {
        theme = "Unknown";
    }
#elif __APPLE__
    osName = "macOS";

    const char* langEnv = getenv("LANG");

    configPath = "/Library/Application Support/Log4Daily";

    FILE* pipe = popen("defaults read -g AppleInterfaceStyle", "r");
    if (!pipe) {
        theme = "Unknown";
    }
    else {
        char buffer[128];
        std::string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
        pclose(pipe);
        theme = (result.find("Dark") != std::string::npos) ? "Dark" : "Light";
    }
#elif defined(__linux__)
    osName = "Linux";

    const char* langEnv = getenv("LANG");
    language = (langEnv != nullptr) ? std::string(langEnv) : "Nieznany";

    const char* configDir = getenv("XDG_CONFIG_HOME");
    const char* homeDir = getenv("HOME");
    
    if (configDir == nullptr && homeDir != nullptr) {
        configPath = std::string(homeDir) + "/.config/log4daily";
    }
    else if (configDir != nullptr) {
        configPath = std::string(configDir) + "/Log4Daily";
    } else {
        std::cerr << "Error getting user home directory" << std::endl;
        exit(1);
    }

    const char* gtkTheme = getenv("GTK_THEME");
    if (gtkTheme != nullptr) {
        theme = (std::string(gtkTheme).find("dark") != std::string::npos) ? "Dark" : "Light";
    }
    else {
        theme = "Unknown";
    }
#else
    osName = "Nieznany system operacyjny";
	throw std::runtime_error("Unsupported operating system");
#endif

	if (theme == "Unknown") {
		theme = "Dark";
	}

    return SystemConfiguration(osName, language, configPath, theme);
}

ConfigFolderStatus ManageConfig::createConfigFolder(const std::string& path) {
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        if (MKDIR(path.c_str()) == 0) {
            std::cout << "Config folder created successfully: " << path << std::endl;
            return ConfigFolderStatus::FOLDER_CREATED;
        } else {
            std::cout << "Error creating config folder: " << path << std::endl;
            return ConfigFolderStatus::FOLDER_ERROR;
        }
    } else if (info.st_mode & S_IFDIR) {
        return ConfigFolderStatus::FOLDER_EXISTS;
    } else {
        return ConfigFolderStatus::FOLDER_ERROR;
    }
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

ConfigFolderStatus ManageConfig::prepareConfigFile() {
	SystemConfiguration configuration = getSystemConfiguration();
    ConfigFolderStatus status = createConfigFolder(configuration.configPath);

	Settings settings;
	settings.readedSystemConfiguration = configuration;
    switch (str2int(configuration.systemLanguage.c_str())) {
	case str2int("en-US"):
		settings.selectedLanguage = SupportedLanguages::English;
		break;
	case str2int("pl-PL"):
		settings.selectedLanguage = SupportedLanguages::Polish;
		break;
	default:
		settings.selectedLanguage = SupportedLanguages::English;
		break;
    }

	settings.selectedTheme = (configuration.systemTheme == "Light") ? Themes::Light : Themes::Dark;

    if (status == ConfigFolderStatus::FOLDER_CREATED) {
        std::ofstream configFile(configuration.configPath + "/" + configFileName);
		if (!configFile.is_open()) {
			std::cerr << "Error creating config file" << std::endl;
			return ConfigFolderStatus::FOLDER_ERROR;
		}

		nlohmann::json settingsJson = SettingsToJson(settings);
		configFile << settingsJson.dump(4);
        configFile.close();
    } 

    return status;
}

nlohmann::json ManageConfig::SystemConfigurationToJson(const SystemConfiguration& config) {
	nlohmann::json j;
	j["OsName"] = config.system;
	j["Language"] = config.systemLanguage;
	j["Theme"] = config.systemTheme;
	j["ConfigPath"] = config.configPath;
	return j;
}

SystemConfiguration ManageConfig::JsonToSystemConfiguration(const nlohmann::json& j) {
	SystemConfiguration config;
	config.system = j["OsName"];
	config.systemLanguage = j["Language"];
	config.systemTheme = j["Theme"];
	config.configPath = j["ConfigPath"];
	return config;
}

nlohmann::json ManageConfig::SupportedLanguagesToJson(const SupportedLanguages& lang) {
	nlohmann::json j;
	j["Language"] = lang == SupportedLanguages::English ? "English" : "Polish";
	return j;
}

SupportedLanguages ManageConfig::JsonToSupportedLanguages(const nlohmann::json& j) {
	std::string lang = j["Language"];
	return lang == "English" ? SupportedLanguages::English : SupportedLanguages::Polish;
}

nlohmann::json ManageConfig::ThemesToJson(const Themes& theme) {
	nlohmann::json j;
	j["Theme"] = theme == Themes::Light ? "Light" : "Dark";
	return j;
}
Themes ManageConfig::JsonToThemes(const nlohmann::json& j) {
	std::string theme = j["Theme"];
	return theme == "Light" ? Themes::Light : Themes::Dark;
}

nlohmann::json ManageConfig::SettingsToJson(const Settings& settings) {
	nlohmann::json j;
	j["ListOfFilesNames"] = settings.listOfFilesNames;
	j["SelectedLanguage"] = SupportedLanguagesToJson(settings.selectedLanguage);
	j["SelectedTheme"] = ThemesToJson(settings.selectedTheme);
	j["SystemConfiguration"] = SystemConfigurationToJson(settings.readedSystemConfiguration);
	return j;
}
Settings ManageConfig::JsonToSettings(const nlohmann::json& j) {
	Settings settings;
	settings.listOfFilesNames = j["ListOfFilesNames"];
	settings.selectedLanguage = JsonToSupportedLanguages(j["SelectedLanguage"]);
	settings.selectedTheme = JsonToThemes(j["SelectedTheme"]);
	settings.readedSystemConfiguration = JsonToSystemConfiguration(j["SystemConfiguration"]);
    return settings;
}
