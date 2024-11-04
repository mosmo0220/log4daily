#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

std::vector<std::string> todos{"Task 1", "Task 2", "Task 3"};
std::vector<std::string> milestones{"Milestone 1", "Milestone 2", "Milestone 3"};
std::vector<std::string> diary{"Entry 1", "Entry 2", "Entry 3"};
std::vector<std::string> settings{"Setting 1", "Setting 2", "Setting 3"};
std::vector<std::string> about{"Info 1", "Info 2", "Info 3"};

Component CreateComponent(const std::vector<std::string>& entries, int& selected, const std::string& content) {
    return Renderer(Radiobox(&entries, &selected), [content] { return text(content); });
}

void SaveFile() {
    // Save fileData to file
}

Component CreateExitComponent(ScreenInteractive& screen, int& exit_selected) {
    auto exit_buttons = Container::Horizontal({
        Button("Exit and Save", [&screen, &exit_selected] {
            exit_selected = 1;
            SaveFile();
            screen.Clear();
            screen.ExitLoopClosure()();
        }),
        Button("Exit without Saving", [&screen, &exit_selected] {
            exit_selected = 1;
            screen.Clear();
            screen.ExitLoopClosure()();
        })
    });

    return Renderer(exit_buttons, [=] {
        return vbox({
            text("Are you sure you want to exit?"),
            exit_buttons->Render()
        });
    });
}

int RenderUI(MainThread mainThread) {
    std::cout << "Rendering UI..." << std::endl;

    FileData fileData = mainThread.getOpenedFile();

    std::vector<std::string> tab_labels{
        "Todos", "Milestones", "Diary", "Settings", "About", "Exit"
    };

    int selected_tab = 0;
    auto tab_toggle = Toggle(&tab_labels, &selected_tab);

    int todos_selected = 0;
    int milestones_selected = 0;
    int diary_selected = 0;
    int settings_selected = 0;
    int about_selected = 0;
    int exit_selected = 0;

    auto screen = ftxui::ScreenInteractive::Fullscreen();

    auto tab_container = Container::Tab(
        {
            CreateComponent(todos, todos_selected, "Todos Content"),
            CreateComponent(milestones, milestones_selected, "Milestones Content"),
            CreateComponent(diary, diary_selected, "Diary Content"),
            CreateComponent(settings, settings_selected, "Settings Content"),
            CreateComponent(about, about_selected, "About Content"),
            CreateExitComponent(screen, exit_selected)
        },
        &selected_tab
    );

    auto container = Container::Vertical({
        tab_toggle,
        tab_container,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Log4Daily") | bold | hcenter,
            separator(),
            tab_toggle->Render(),
            tab_container->Render(),
        }) | border;
    });

    screen.Loop(renderer);
    return 0;
}
