#include <thread>

#include "../Headers/Storage/manageConfig.h"
#include "../Headers/applicationManager.h"
#include "../Headers/uiRenderer.h"

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
    ManageConfig manageConfig;
    ConfigFolderStatus confingRespond = manageConfig.prepareConfigFile();

    if (confingRespond == ConfigFolderStatus::CREATED) {
        std::cerr << "Configuration folder created successfully" << std::endl;
        return 1;
    }

    if (confingRespond == ConfigFolderStatus::ERROR) {
        std::cerr << "Problem acured while creating config folder" << std::endl;
        return 1;
    }
    
    ApplicationManager applicationManager(manageConfig.getConfigFolderPath() + "/", manageConfig.configFileName);
    CommandType respond = applicationManager.run(argc, argv);
    
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
