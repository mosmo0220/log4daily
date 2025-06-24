#include "../../Headers/Interface/exitComponent.h"

#include "../../Headers/applicationManager.h"
#include "../../../l4dFiles/out/l4dFiles.hpp"
#include "../../Headers/uiRenderer.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component ExitComponent::exitComponent(uiRenderer& UI) {
    auto exitButtons = ftxui::Container::Horizontal({
        Button("Save Changes", [&UI] {
            UI.saveFileData();
        }) | size(WIDTH, LESS_THAN, 20) | size(HEIGHT, LESS_THAN, 3) | color(Color::Green),
        Button("Discard Changes", [&UI] {
            UI.discardFileData();
        }) | size(WIDTH, LESS_THAN, 20) | size(HEIGHT, LESS_THAN, 3) | color(Color::Yellow),
        Button("Exit Log4Daily", [&UI] {
            UI.exit();
        }) | size(WIDTH, LESS_THAN, 20) | size(HEIGHT, LESS_THAN, 3) | color(Color::Red),
    });


    return ftxui::Container::Vertical({
        ftxui::Renderer([] { return text("Select action:") | hcenter | bold; }),
        exitButtons | hcenter
    });
}