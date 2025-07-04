#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <string>

// Log4daily components
#include "./applicationManager.h"

#include "../../l4dFiles/out/l4dFiles.hpp"

// FTXUI component
#include "ftxui/component/component.hpp"

using namespace ftxui;

class uiRenderer {
    public:
        uiRenderer(ApplicationManager *_applicationManager);
        int renderUI(int isReload);
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