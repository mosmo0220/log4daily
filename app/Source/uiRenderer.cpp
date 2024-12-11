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

int uiRenderer::renderUI(int isReload) {
    int minHeight = 40;
    int minWidth = 125;

    auto screen = ScreenInteractive::Fullscreen();
    screen_ptr = &screen;

    std::vector<std::string> tabLabels{
        "Todos", "Milestones", "Diary", "Exit"
    };

    int selectedTab = 0;
    auto tabToggle = Toggle(&tabLabels, &selectedTab);

    std::string notificationText = "";

    if (isReload == 1) {
        selectedTab = 3;
        notificationText = "Changes has been discarded.";
    }
    else if (isReload == 2) {
        selectedTab = 3;
        notificationText = "Changes has been saved.";
    }

    auto centeredTabToggle = Renderer(tabToggle, [&tabToggle] {
        return hbox({
            filler(),                
            tabToggle->Render(),    
            filler()                
        }) | center;            
    });

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

    auto notification = ftxui::Renderer([&notificationText] {
        if (notificationText.empty()) {
            return ftxui::text("");
        }
        std::thread([&notificationText] {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            notificationText = "";
        }).detach();
        return ftxui::text(notificationText);
    });


    auto content = [&]() -> Element {
        if (screen.dimx() < minWidth || screen.dimy() < minHeight) {
            return center(vbox({
                text("Log4Daily") | bold | hcenter,
                text("Minimal width: " + std::to_string(screen.dimx()) + " / " + std::to_string(minWidth)),
                text("Minimal height: " + std::to_string(screen.dimy()) + " / " + std::to_string(minHeight)),
                separator(),
                text("Expand the window to see the content."),
            })) | border;
        }
        return vbox({
            text("Log4Daily") | bold | hcenter,
            separator(),
            centeredTabToggle->Render(),
            filler(),
            tabContainer->Render(),
            notification->Render() | hcenter,
            filler(),
        }) | border | size(WIDTH, EQUAL, 200) | size(HEIGHT, EQUAL, 50) | center \
            | color(LinearGradient().Angle(45).Stop(Color::DeepSkyBlue1).Stop(Color::LightSkyBlue1).Stop(Color::LightSkyBlue1).Stop(Color::DeepSkyBlue1))
            | bgcolor(Color::Default);;
    };

    auto renderer = Renderer(container, [&content] {
        return content();
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
    reloadUI = 2;
}

void uiRenderer::exit() {
    screen_ptr->Clear();
    screen_ptr->ExitLoopClosure()();
    reloadUI = 0;
}
