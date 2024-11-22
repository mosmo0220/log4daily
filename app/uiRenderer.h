#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <string>

// Log4daily components
#include "./Storage/localStorage.h"
#include "applicationManager.h"

// FTXUI component
#include "ftxui/component/component.hpp"

using namespace ftxui;

class uiRenderer {
    public:
        uiRenderer(ApplicationManager *_applicationManager);
        Component createExitComponent(ScreenInteractive& screen, int& exitSelected);
        int renderUI();
        std::string getExitMessage();
    private:
        ApplicationManager *applicationManager;
        FileData inUseFileData;
        bool exitedWithoutSaving = false;
};

#endif // UI_RENDERER_H