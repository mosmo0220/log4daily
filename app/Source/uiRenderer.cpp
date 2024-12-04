#include "../Headers/uiRenderer.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>

// Log4daily components
#include "../Headers/Storage/localStorage.h"
#include "../Headers/applicationManager.h"

// FTXUI components
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

// UI components
#include "../Headers/Interface/todosComponent.h"
#include "../Headers/Interface/milestonesComponent.h"
#include "../Headers/Interface/diaryComponent.h"
#include "../Headers/Interface/exitComponent.h"

using namespace ftxui;

uiRenderer::uiRenderer(ApplicationManager *_applicationManager) : inUseFileData(_applicationManager->getOpenedFile()) {
    applicationManager = _applicationManager;
    inUseFileDataBeforeSave = inUseFileData;
}

int uiRenderer::renderUI() {
    std::vector<std::string> tabLabels{
        "Todos", "Milestones", "Diary", "Exit"
    };

    int selectedTab = 0;
    auto tabToggle = Toggle(&tabLabels, &selectedTab);

    auto centeredTabToggle = Renderer(tabToggle, [&tabToggle] {
        return hbox({
            filler(),                
            tabToggle->Render(),    
            filler()                
        }) | center;            
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen_ptr = &screen;

    TodosComponent todosComponent;
    MilestonesComponent milestonesComponent;
    DiaryComponent diaryComponent;
    ExitComponent exitComponent;

    auto tabContainer = Container::Tab(
        {
            todosComponent.renderTodosComponent(&inUseFileData),
            milestonesComponent.renderMilestonesComponent(&inUseFileData),
            diaryComponent.diaryComponent(&inUseFileData),
            exitComponent.exitComponent(*this)
        },
        &selectedTab
    );

    auto container = Container::Vertical({
        tabToggle,
        tabContainer,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Log4Daily") | bold | hcenter,
            separator(),
            centeredTabToggle->Render(),
            filler(),
            tabContainer->Render(),
            filler(),
        }) | border | size(WIDTH, EQUAL, 200) | size(HEIGHT, EQUAL, 50) | center \
            | color(LinearGradient().Angle(45).Stop(Color::DeepSkyBlue1).Stop(Color::LightSkyBlue1).Stop(Color::LightSkyBlue1).Stop(Color::DeepSkyBlue1))
            | bgcolor(Color::Default);
    });

    screen.Loop(renderer);
    return reloadUI;
}

void uiRenderer::discardFileData() {
    inUseFileData = inUseFileDataBeforeSave;

    screen_ptr->Clear();
    screen_ptr->ExitLoopClosure()();
    reloadUI = 1;
}

void uiRenderer::saveFileData() {
    applicationManager->updateFileData(inUseFileData);

    screen_ptr->Clear();
    screen_ptr->ExitLoopClosure()();
    reloadUI = 1;
}

void uiRenderer::exit() {
    screen_ptr->Clear();
    screen_ptr->ExitLoopClosure()();
    reloadUI = 0;
}
