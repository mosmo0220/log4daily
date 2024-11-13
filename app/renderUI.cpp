#include <memory>
#include <string>
#include <vector>
#include <functional>

// Log4daily components
#include "./Storage/localStorage.h"
#include "mainThread.h"

// FTXUI components
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

// UI components
#include "UIComponents/createComponent.cpp"
#include "UIComponents/todosComponent.cpp"
#include "UIComponents/milestonesComponent.cpp"
#include "UIComponents/diaryComponent.cpp"
#include "UIComponents/aboutComponent.cpp"

using namespace ftxui;

/**
 * @class RenderUI
 * @brief Provides functionalities to render the log4daily UI.
 * 
 * The RenderUI class provides functionalities to render the log4daily UI components
 * and handle user interactions. It includes methods to create and render the UI components,
 * such as the tab container, todos component, milestones component, diary component, and about component.
 */
class RenderUI {
    public:
        RenderUI(MainThread _mainThread) : mainThread(_mainThread), inUseFileData(_mainThread.getOpenedFile()) {}

        /**
         * @brief Creates the exit component.
         * 
         * This function creates the exit component, which displays a confirmation message
         * and options to exit and save or exit without saving.
         */
        Component createExitComponent(ScreenInteractive& screen, int& exitSelected) {
            auto exitButtons = Container::Horizontal({
                Button("Exit and Save", [this, &screen, &exitSelected] {
                    exitSelected = 1;
                    mainThread.updateFileData(inUseFileData);
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
                    text("Are you sure you want to exit?") | hcenter,
                    exitButtons->Render() | hcenter
                });
            });
        }

        /**
         * @brief Renders the UI.
         * 
         * This function renders the UI components and handles the user interactions.
         */
        int renderUI() {
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

            auto tabContainer = Container::Tab(
                {
                    todosComponent(&inUseFileData),
                    milestonesComponent(),
                    diaryComponent(),
                    aboutComponent(),
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
                }) | border;
            });

            screen.Loop(renderer);
            return 0;
        }

        std::string getExitMessage() {
            return !exitedWithoutSaving ? "Exiting and saving" : "Exiting without saving";
        }
    private:
        MainThread mainThread;
        FileData inUseFileData;
        bool exitedWithoutSaving = false;
};
