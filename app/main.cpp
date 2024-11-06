#include "mainThread.cpp"
#include "renderUI.cpp"

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
    MainThread mainThread("/home/zaruuwa/log4daily/log4daily/build/log4daily_files/", "log4daily_config.json");
    CommandType respond = mainThread.run(argc, argv);
    
    std::string message = mainThread.getRespondMessage().empty() ? "" : mainThread.getRespondMessage();
    mainThread.showMessage(respond, message);    

    if (respond == CommandType::New || respond == CommandType::Open) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        RenderUI rUI(mainThread);
        int returnCode = rUI.renderUI();
        std::cout << rUI.getExitMessage() << std::endl;
        return returnCode;
    }
    return 0;
}
