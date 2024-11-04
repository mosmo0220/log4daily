#include "mainThread.cpp"
#include "renderUI.cpp"

int main(int argc, char** argv) {
    MainThread mainThread("/home/zaruuwa/log4daily/log4daily/build/log4daily_files/", "log4daily_config.json");
    CommandType respond = mainThread.run(argc, argv);
    
    std::string message = mainThread.getRespondMessage().empty() ? "" : mainThread.getRespondMessage();
    mainThread.ShowMessage(respond, message);    

    if (respond == CommandType::NEW || respond == CommandType::OPEN) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return RenderUI(mainThread);
    }
    return 0;
}
