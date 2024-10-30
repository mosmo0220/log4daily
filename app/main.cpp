#include "main_thread.cpp"

int main(int argc, char** argv) {
    main_thread mainThread("/home/zaruuwa/log4daily/log4daily/build/log4daily_files/", "log4daily_config.json");
    runThread respond = mainThread.run(argc, argv);

    std::string message = mainThread.getRespondMessage().empty() ? "" : mainThread.getRespondMessage();
    mainThread.ShowMessage(respond, message);    

    return 0;
}
