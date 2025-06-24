#include <thread>

#include "../Headers/applicationManager.h"
#include "../Headers/uiRenderer.h"

#include "../l4dFiles/out/l4dFiles.hpp"

/**
 * @brief The main function of the log4daily application.
 * 
 * This function initializes the main thread of the log4daily application and executes the commands
 * based on the console inputs. It then renders the log4daily UI and displays the exit message.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int The exit code of the application.
 */
int main(int argc, char** argv) {
#ifdef _DEBUG
    bool create = true;
    int argc_DEBUG = 3;
	char* p0 = (char*)"log4daily";
    char* p = (char*)(create ? "--new" : "--open");
	char* p1 = (char*)"test";
    char* argv_DEBUG[] = { p0, p, p1 };
#endif
    
    ManageConfig manageConfig;
    ConfigFolderStatus configRespond = manageConfig.prepareConfigFile();

    if (configRespond == ConfigFolderStatus::FOLDER_ERROR) {
        std::cerr << "Problem acured while creating config folder" << std::endl;
        return 1;
    }

    if (configRespond == ConfigFolderStatus::FOLDER_CREATED) {
        std::cout << "Configuration folder created successfully" << std::endl;
    }
    
    ApplicationManager applicationManager(manageConfig.getSystemConfiguration().configPath + "/", manageConfig.configFileName);
#ifdef _DEBUG
    CommandType respond = applicationManager.run(argc_DEBUG, argv_DEBUG);
#else
    CommandType respond = applicationManager.run(argc, argv);
#endif
    
    std::string message = applicationManager.getRespondMessage().empty() ? "" : applicationManager.getRespondMessage();
    applicationManager.showMessage(respond, message);    

    if (respond == CommandType::New || respond == CommandType::Open) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        applicationManager.registerToday();
        uiRenderer UI(&applicationManager);
        
        int returnCode = 0;
        do {
            returnCode = UI.renderUI(returnCode);
        } while (returnCode != 0);
        return returnCode;
    }
    return 0;
}
