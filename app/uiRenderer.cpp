#include "uiRenderer.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>

// Log4daily components
#include "./Storage/localStorage.h"
#include "applicationManager.h"

// FTXUI components
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

// UI components
#include "Interface/todosComponent.h"
#include "Interface/milestonesComponent.h"

// Incomplete UI components
#include "Interface/diaryComponent.h"
#include "Interface/aboutComponent.h"

using namespace ftxui;

uiRenderer::uiRenderer(ApplicationManager *_applicationManager) : inUseFileData(_applicationManager->getOpenedFile()) {
    applicationManager = _applicationManager;
}

Component uiRenderer::createExitComponent(ScreenInteractive& screen, int& exitSelected) {
    auto exitButtons = Container::Horizontal({
        Button("Exit and Save", [this, &screen, &exitSelected] {
            exitSelected = 1;
            applicationManager->updateFileData(inUseFileData);
            screen.Clear();
            screen.ExitLoopClosure()();
        }),
        Button("Exit without Saving", [this, &screen, &exitSelected] {
            exitSelected = 1;
            exitedWithoutSaving = true;
            screen.Clear();
            screen.ExitLoopClosure()();
        })
    });

    return Renderer(exitButtons, [=] {
        return vbox({
            text("Exit options:") | hcenter,
            exitButtons->Render() | hcenter
        });
    });
}

int uiRenderer::renderUI() {
    std::vector<std::string> tabLabels{
        "Todos", "Milestones", "Diary", "About", "Exit"
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

    int todosSelected = 0;
    int milestonesSelected = 0;
    int diarySelected = 0;
    int settingsSelected = 0;
    int aboutSelected = 0;
    int exitSelected = 0;

    auto screen = ftxui::ScreenInteractive::Fullscreen();

    TodosComponent todosComponent;
    MilestonesComponent milestonesComponent;
    DiaryComponent diaryComponent;
    AboutComponent aboutComponent;

    auto tabContainer = Container::Tab(
        {
            todosComponent.renderTodosComponent(&inUseFileData),
            milestonesComponent.renderMilestonesComponent(&inUseFileData),
            diaryComponent.diaryComponent(),
            aboutComponent.aboutComponent(),
            createExitComponent(screen, exitSelected)
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
    return 0;
}

std::string uiRenderer::getExitMessage() {
    return !exitedWithoutSaving ? "Exiting and saving" : "Exiting without saving";
}