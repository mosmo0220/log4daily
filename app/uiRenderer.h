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
        int renderUI();
        void discardFileData();
        void saveFileData();
        void exit();
    private:
        ApplicationManager *applicationManager;
        ScreenInteractive *screen_ptr;
        FileData inUseFileData;
        FileData inUseFileDataBeforeSave;
        int reloadUI = 0;
};

#endif // UI_RENDERER_H