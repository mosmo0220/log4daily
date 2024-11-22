#include <thread>

#include "./Storage/manageConfig.cpp"
#include "applicationManager.h"
#include "uiRenderer.h"

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
    std::string configRespond = manageConfig();
    if (configRespond == "Problem acured while creating config folder" || configRespond == "Error!") {
        std::cerr << configRespond << std::endl;
        return 1;
    }
    
    ApplicationManager applicationManager(configRespond + "/", configFileName);
    CommandType respond = applicationManager.run(argc, argv);
    
    std::string message = applicationManager.getRespondMessage().empty() ? "" : applicationManager.getRespondMessage();
    applicationManager.showMessage(respond, message);    

    if (respond == CommandType::New || respond == CommandType::Open) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        uiRenderer UI(&applicationManager);
        int returnCode = UI.renderUI();
        std::cout << UI.getExitMessage() << std::endl;
        return returnCode;
    }
    return 0;
}
