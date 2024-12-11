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
    std::string configRespond = manageConfig.prepareConfigFile();
    if (configRespond == "Problem acured while creating config folder" || configRespond == "Error!") {
        std::cerr << configRespond << std::endl;
        return 1;
    }
    
    ApplicationManager applicationManager(configRespond + "/", manageConfig.configFileName);
    CommandType respond = applicationManager.run(argc, argv);
    
    std::string message = applicationManager.getRespondMessage().empty() ? "" : applicationManager.getRespondMessage();
    applicationManager.showMessage(respond, message);    

    if (respond == CommandType::New || respond == CommandType::Open) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
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
